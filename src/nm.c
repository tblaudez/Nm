/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nm.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:05:49 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/14 15:16:27 by tblaudez      ########   odam.nl         */
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

t_list *merge_list(t_list *left, t_list *right)
{
	t_list *result = NULL;
	t_list *tmp;

	while (left && right) {
		if (ft_strcmp(((t_symbol*)left->content)->name, ((t_symbol*)right->content)->name) <= 0) {
			tmp = left->next;
			left->next = NULL;
			ft_lstadd_back(&result, left);
			left = tmp;
		} else {
			tmp = right->next;
			right->next = NULL;
			ft_lstadd_back(&result, right);
			right = tmp;
		}
	}

	while (left) {
		tmp = left->next;
		left->next = NULL;
		ft_lstadd_back(&result, left);
		left = tmp;
	}
	while (right) {
		tmp = right->next;
		right->next = NULL;
		ft_lstadd_back(&result, right);
		right = tmp;
	}

	return result;
}

t_list *merge_sort(t_list *lst)
{
	int lst_len = ft_lstsize(lst);

	if (lst_len <= 1)
		return lst;
	
	t_list *left = NULL;
	t_list *right = NULL;
	t_list *tmp;

	for(int i = 0; lst; i++) {
		tmp = lst->next;
		lst->next = NULL;
		ft_lstadd_back((i < lst_len / 2 ? &left : &right), lst);
		lst = tmp;
	}
	
	left = merge_sort(left);
	right = merge_sort(right);

	t_list *result = merge_list(left, right);
	return result;
}

void display_symbol(void *ptr)
{
	t_symbol *symbol = (t_symbol*)ptr;
	ft_fprintf(STDOUT_FILENO, "%016x X %s\n", symbol->value, symbol->name);
}

int	main(int argc, char *argv[])
{
	struct stat		file_stat;
	const char		*filename = (argc < 2 ? "a.out" : argv[1]);
	char			*mapping;
	int				fd;

	if ((fd = open(filename, O_RDONLY)) == -1) {
		ft_fprintf(STDERR_FILENO, "ft_nm: open: Could not open '%s'\n", filename);
		return 1;
	}
	fstat(fd, &file_stat);
	if (S_ISDIR(file_stat.st_mode)) {
		ft_fprintf(STDERR_FILENO, "ft_nm: Warning: '%s' is a directory\n", filename);
		close(fd);
		return 1;
	}
	if ((mapping = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		ft_fprintf(STDERR_FILENO, "ft_nm: mmap: Could not map memory\n");
		close(fd);
		return 1;
	}
	if (ft_strncmp(ELFMAG, mapping, 4) != 0) {
		ft_fprintf(STDERR_FILENO, "%s: file format not recognized\n", filename);
		close(fd);
		munmap(mapping, file_stat.st_size);
		return 1;
	}

	
	t_list *symbol_list = NULL;
	Elf64_Ehdr *ehdr = (Elf64_Ehdr*)mapping; // ELF header
	Elf64_Shdr *shdr = (Elf64_Shdr*)(mapping + ehdr->e_shoff); // Section header is `e_shoff` after ELF header
	
	for (int i = 0; i < ehdr->e_shnum; i++) { // Looping over all sections
		if (shdr[i].sh_type == SHT_SYMTAB) { // If section is a Symbol table
			Elf64_Sym *symtab = (Elf64_Sym*)(mapping + shdr[i].sh_offset); // Get Symbol table using `sh_offset`
			int symtab_len = shdr[i].sh_size / shdr[i].sh_entsize; // Size of section divided by size of entry gives number of entries
			char *symtab_strtab = (char*)(mapping + shdr[shdr[i].sh_link].sh_offset); // Symbol table has its string table at index `sh_link`
			
			for (int j = 0; j < symtab_len; j++) {
				if (!ft_strcmp(symtab_strtab + symtab[j].st_name, ""))
					continue;
				t_symbol *symbol = ft_memalloc(sizeof(t_symbol));
				symbol->name = symtab_strtab + symtab[j].st_name;
				symbol->type = ELF64_ST_TYPE(symtab[j].st_info);
				symbol->value = symtab[j].st_value;
				ft_lstadd_back(&symbol_list, ft_lstnew(symbol));
			}
		}
	}

	symbol_list = merge_sort(symbol_list);
	ft_lstiter(symbol_list, display_symbol);

	ft_lstclear(&symbol_list, free);
	munmap(mapping, file_stat.st_size);
	close(fd);
	return 0;
}