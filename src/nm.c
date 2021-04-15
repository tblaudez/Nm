/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nm.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:05:49 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/15 10:32:51 by tblaudez      ########   odam.nl         */
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

int file_fd = -1;
char *file_map = NULL;
t_list *symbol_list = NULL;
struct stat file_stat;

static inline int compare_symbols(void *a, void *b)
{
	return ft_strcmp(((t_symbol*)a)->name, ((t_symbol*)b)->name);
}

static void display_symbol(void *ptr)
{
	t_symbol *symbol = (t_symbol*)ptr;
	ft_fprintf(STDOUT_FILENO, "%016x X %s\n", symbol->value, symbol->name);
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

static t_symbol *create_symbol(char *name, unsigned char type, uint64_t value)
{
	t_symbol *symbol = (t_symbol*)ft_memalloc(sizeof(t_symbol));

	symbol->name = name;
	symbol->type = type;
	symbol->value = value;

	return symbol;
}

static void fill_symbol_list()
{
	Elf64_Ehdr *ehdr = (Elf64_Ehdr*)file_map; // ELF header
	Elf64_Shdr *shdr = (Elf64_Shdr*)(file_map + ehdr->e_shoff); // Section header is `e_shoff` after ELF header

	for (int i = 0; i < ehdr->e_shnum; i++) { // Looping over all sections
		if (shdr[i].sh_type == SHT_SYMTAB) { // If section is a Symbol table
			Elf64_Sym *symtab = (Elf64_Sym*)(file_map + shdr[i].sh_offset); // Get Symbol table using `sh_offset`
			int symtab_len = shdr[i].sh_size / shdr[i].sh_entsize; // Size of section divided by size of entry gives number of entries
			char *symtab_strtab = (char*)(file_map + shdr[shdr[i].sh_link].sh_offset); // Symbol table has its string table at index `sh_link`
			
			for (int j = 0; j < symtab_len; j++) {
				if (!ft_strcmp(symtab_strtab + symtab[j].st_name, ""))
					continue;
				t_symbol *symbol = create_symbol(symtab_strtab + symtab[j].st_name, \
					ELF64_ST_TYPE(symtab[j].st_info), symtab[j].st_value);
				ft_lstadd_back(&symbol_list, ft_lstnew(symbol));
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
	
	// Get symbols and sort them alphabetically
	fill_symbol_list();
	if (!symbol_list) {
		ft_fprintf(STDOUT_FILENO, "ft_nm: %s: no symbols\n", filename);
		exit(EXIT_FAILURE);
	}
	merge_sort_list(&symbol_list, compare_symbols);
	
	// Display symbols
	ft_lstiter(symbol_list, display_symbol); 

	return 0;
}