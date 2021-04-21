/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nm.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:05:49 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/20 11:20:14 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include "libft.h"
#include "ft_fprintf.h"

#include <sys/types.h> // open, fstat
#include <sys/stat.h> // open, fstat
#include <fcntl.h> // open
#include <unistd.h> // fstat, close
#include <sys/mman.h> // mmap, munmap
#include <elf.h> // Elf64_Ehdr, Elf64_Shdr, Elf64_Sym
#include <stdlib.h> // free
#include <stdbool.h> // bool

int file_fd = -1;
char *file_map = NULL;
t_list *symbol_list = NULL;
Elf64_Ehdr *ehdr = NULL;
Elf64_Shdr *shdr = NULL;
struct stat file_stat;

static inline int compare_symbols(void *a, void *b)
{
	return ft_strcmp(((t_symbol*)a)->name, ((t_symbol*)b)->name);
}

static void display_symbol(void *ptr)
{
	const t_symbol *symbol = (t_symbol*)ptr;
	const unsigned char bind = ELF64_ST_BIND(symbol->symbol->st_info);
	const bool local = (bind == STB_LOCAL);
	const unsigned char type = ELF64_ST_TYPE(symbol->symbol->st_info);
	const uint16_t shndx = symbol->symbol->st_shndx;
	const Elf64_Shdr *section = shdr + shndx;
	char c;

	if (shndx >= ehdr->e_shnum)
		return;

	// Symbol is absolute
	if (shndx == SHN_ABS)
		c = 'A'; 

	// Symbol is in the uninitialized data section
	else if (section->sh_type == SHT_NOBITS && section->sh_flags == (SHF_ALLOC|SHF_WRITE))
		c = (local ? 'b' : 'B'); 

	// Symbol is common
	else if (shndx == SHN_COMMON)
		c = 'C';
	
	// Symbol is in the initialized data section
	else if (section->sh_type == SHT_PROGBITS && section->sh_flags == (SHF_ALLOC|SHF_WRITE))
		c = (local ? 'd' : 'D');

	// G g - Initialized data section for small objects

	// i - Symbol is an indirect function

	// I - Symbol is an indirect reference to another symbol

	// Symbol is a debugging symbol
	else if (section->sh_type == SHT_PROGBITS && section->sh_flags == 0)
		c = 'N';

	// P - Symbols is in a stack unwind section

	// Symbol is in read-only data section
	else if (section->sh_type == SHT_PROGBITS && section->sh_flags == SHF_ALLOC)
		c = (local ? 'r' : 'R');
	
	// S - Symbol is in uninitialized or zero-initialized data section for small objects

	// Symbol is in the text (code) section.
	else if (section->sh_type == SHT_PROGBITS && section->sh_flags == (SHF_ALLOC|SHF_EXECINSTR))
		c = (local ? 't' : 'T');

	// Symbol is undefined
	else if (shndx == SHN_UNDEF)
		c = 'U';
	
	// Symbol is unique global
	else if (bind == STB_GNU_UNIQUE)
		c = 'u';
	
	// Symbol is weak object
	else if (bind == STB_WEAK && type == STT_OBJECT)
		c = (shndx == SHN_UNDEF ? 'v' : 'V');
	
	// Symbol is a weak symbol
	else if (bind == STB_WEAK)
		c = (shndx == SHN_UNDEF ? 'w' : 'W');
	
	// Symbol type is unknown, or object file format specific
	else
		c = '?';
	
	
	if (ft_strchr("Uwv", c))
		ft_fprintf(STDOUT_FILENO, "%16c %c %s\n", ' ', c, symbol->name);
	else if (symbol->symbol->st_value != 0)
		ft_fprintf(STDOUT_FILENO, "%016x %c %s\n", symbol->symbol->st_value, c, symbol->name);
}

void open_and_map_file(const char *filename)
{
	if ((file_fd = open(filename, O_RDONLY)) == -1) {
		ft_fprintf(STDERR_FILENO, "ft_nm: open: Could not open '%s'\n", filename);
		exit(EXIT_FAILURE);
	}

	fstat(file_fd, &file_stat);
	
	if (S_ISDIR(file_stat.st_mode)) {
		ft_fprintf(STDERR_FILENO, "ft_nm: Warning: '%s' is a directory\n", filename);
		exit(EXIT_FAILURE);
	}
	
	if ((file_map = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, file_fd, 0)) == MAP_FAILED) {
		ft_fprintf(STDERR_FILENO, "ft_nm: mmap: Could not map memory\n");
		exit(EXIT_FAILURE);
	}
	
	if (ft_strncmp(ELFMAG, file_map, 4) != 0) {
		ft_fprintf(STDERR_FILENO, "%s: file format not recognized\n", filename);
		exit(EXIT_FAILURE);
	}
}

static t_symbol *create_symbol(Elf64_Sym *symbol, const char *strtab)
{
	t_symbol *custom_symbol = (t_symbol*)ft_memalloc(sizeof(t_symbol));

	custom_symbol->symbol = symbol;
	custom_symbol->name = strtab + symbol->st_name;

	return custom_symbol;
}

static void fill_symbol_list()
{
	ehdr = (Elf64_Ehdr*)file_map; // ELF header
	shdr = (Elf64_Shdr*)(file_map + ehdr->e_shoff); // Section header is `e_shoff` after ELF header

	for (int i = 0; i < ehdr->e_shnum; i++) { // Looping over all sections
		if (shdr[i].sh_type == SHT_SYMTAB) { // If section is a Symbol table
			Elf64_Sym *symtab = (Elf64_Sym*)(file_map + shdr[i].sh_offset); // Get Symbol table using `sh_offset`
			int symtab_len = shdr[i].sh_size / shdr[i].sh_entsize; // Size of section divided by size of entry gives number of entries
			char *symtab_strtab = (char*)(file_map + shdr[shdr[i].sh_link].sh_offset); // Symbol table has its string table at index `sh_link`
			
			for (int j = 0; j < symtab_len; j++) {
				if (!ft_strcmp(symtab_strtab + symtab[j].st_name, ""))
					continue;
				ft_lstadd_back(&symbol_list, ft_lstnew(create_symbol(symtab + j, symtab_strtab)));
			}
		}
	}
}

void cleanup(void)
{
	if (file_fd != -1)
		close(file_fd);
	if (file_map != NULL)
		munmap(file_map, file_stat.st_size);
	ft_lstclear(&symbol_list, free);
}

int	main(int argc, char *argv[])
{
	const char *filename = (argc < 2 ? "a.out" : argv[1]);

	// Prepare cleanup
	atexit(cleanup);

	// Map file in memory
	open_and_map_file(filename);
	
	// Get symbols
	fill_symbol_list();
	if (!symbol_list) {
		ft_fprintf(STDOUT_FILENO, "ft_nm: %s: no symbols\n", filename);
		return 1;
	}
	
	// Sort list
	merge_sort_list(&symbol_list, compare_symbols);
	
	// Display symbols
	ft_lstiter(symbol_list, display_symbol);

	return 0;
}