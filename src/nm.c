/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nm.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:05:49 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/26 12:05:40 by tblaudez      ########   odam.nl         */
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

Elf64_Ehdr *ehdr = NULL;
Elf64_Shdr *shdr = NULL;

// G g - Initialized data section for small objects
// i - Symbol is an indirect function
// I - Symbol is an indirect reference to another symbol
// P - Symbols is in a stack unwind section
// S - Symbol is in uninitialized or zero-initialized data section for small objects

static char get_symbol_letter(const t_symbol *symbol)
{
	const Elf64_Shdr *section = &shdr[symbol->symbol->st_shndx];
	const Elf64_Shdr *shstrtab = &shdr[ehdr->e_shstrndx];
	char *ptr = (char*)ehdr + shstrtab->sh_offset;
	char *name = ptr + section->sh_name;

	// Absolute symbol
	if (symbol->symbol->st_shndx == SHN_ABS)
		return 'A';
	
	// BSS data section
	if (section->sh_type == SHT_NOBITS && section->sh_flags == (SHF_WRITE|SHF_ALLOC))
		return (ELF64_ST_BIND(symbol->symbol->st_info) == STB_LOCAL ? 'b' : 'B');
	
	// Read only data section
	if (section->sh_type == SHT_NOBITS && section->sh_flags == SHF_ALLOC)	
		return (ELF64_ST_BIND(symbol->symbol->st_info) == STB_LOCAL ? 'r' : 'R');
	
	// Common symbol
	if (symbol->symbol->st_shndx == SHN_COMMON)
		return 'C';
	
	// Initialized data section
	if (section->sh_type == SHT_PROGBITS && section->sh_flags == (SHF_ALLOC|SHF_WRITE))
		return (ELF64_ST_BIND(symbol->symbol->st_info) == STB_LOCAL ? 'd' : 'D');
	
	// Debugging symbol
	if (section->sh_type == SHT_PROGBITS && section->sh_flags == 0)
		return 'N';

	// Text section
	if (section->sh_type == SHT_PROGBITS && section->sh_flags == (SHF_ALLOC|SHF_EXECINSTR))
		return (ELF64_ST_BIND(symbol->symbol->st_info) == STB_LOCAL ? 't' : 'T');
	
	// Unique global symbol
	if (ELF64_ST_BIND(symbol->symbol->st_info) == STB_GNU_UNIQUE)
		return 'u';
	
	// Weak object
	if (ELF64_ST_BIND(symbol->symbol->st_info) == STB_WEAK && ELF64_ST_TYPE(symbol->symbol->st_info) == STT_OBJECT)
		return (symbol->symbol->st_shndx == SHN_UNDEF ? 'v' : 'V');
	
	// Weak symbol
	if (ELF64_ST_BIND(symbol->symbol->st_info) == STB_WEAK)
		return (symbol->symbol->st_shndx == SHN_UNDEF ? 'w' : 'W');
	
	// Undefined symbol
	if (symbol->symbol->st_shndx == SHN_UNDEF)
		return 'U';
	
	return '?';
}
	

static void display_symbols(void *ptr)
{
	const t_symbol *symbol = (t_symbol*)ptr;
	char c = get_symbol_letter(symbol);

	if (ft_strchr("Uwv", c))
		ft_fprintf(STDOUT_FILENO, "%16c %c %s\n", ' ', c, symbol->name);
	else if (symbol->symbol->st_value != 0)
		ft_fprintf(STDOUT_FILENO, "%016x %c %s\n", symbol->symbol->st_value, c, symbol->name);
}

static t_list *extract_symbols_from_table(const Elf64_Shdr *section)
{
	const Elf64_Sym *symtab = (void*)ehdr + section->sh_offset;
	const char *symtab_strtab = (void*)ehdr + shdr[section->sh_link].sh_offset;
	size_t symtab_num = section->sh_size / section->sh_entsize;
	
	t_list *symbol_list = NULL;

	for(size_t i = 0; i < symtab_num; i++) {
		if (ft_strcmp(symtab_strtab + symtab[i].st_name, "") != 0) {
			t_symbol *symbol = create_symbol(symtab + i, symtab_strtab + symtab[i].st_name);
 			t_list *node = ft_lstnew((void*)symbol);
			ft_lstadd_back(&symbol_list, node);
		}
	}

	return symbol_list;
}

void ft_nm(const char *mapping)
{
	ehdr = (Elf64_Ehdr*)mapping;
	shdr = (void*)ehdr + ehdr->e_shoff;

	t_list *symbol_list;

	for(int i = 0; i < ehdr->e_shnum; i++) {
		if (shdr[i].sh_type == SHT_SYMTAB) {
			symbol_list = extract_symbols_from_table(shdr + i);
			merge_sort_list(&symbol_list, compare_symbols);
			ft_lstiter(symbol_list, display_symbols);
			ft_lstclear(&symbol_list, free);
		}
	}
}


int main(int argc, char *argv[])
{
	(void)argc;
	struct stat file_stat;
	char *mapping;
	int fd;

	fd = open(argv[1], O_RDONLY);
	fstat(fd, &file_stat);
	mapping = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	ft_nm(mapping);

	munmap(mapping, file_stat.st_size);
	close(fd);

	return 0;
}