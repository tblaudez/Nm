/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   elf64.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/27 10:08:49 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/30 11:55:28 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_fprintf.h"
#include <elf.h> // elf
#include <stddef.h> // NULL

static const Elf64_Ehdr	*ehdr = NULL;
static const Elf64_Shdr	*shdr = NULL;
static const char 		*symtab_strtab = NULL;


static inline int elf_64_compare_symbols(void *a, void *b)
{
	return ft_strcmp(symtab_strtab + ((Elf64_Sym*)a)->st_name, symtab_strtab + ((Elf64_Sym*)b)->st_name);
}

static char elf_64_get_type(const Elf64_Sym *symbol)
{
	char c;

	if (ELF64_ST_BIND(symbol->st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF64_ST_BIND(symbol->st_info) == STB_WEAK) {
		c = 'W';
		if (symbol->st_shndx == SHN_UNDEF)
			c = 'w';			
	}
	else if (ELF64_ST_BIND(symbol->st_info) == STB_WEAK && ELF64_ST_TYPE(symbol->st_info) == STT_OBJECT) {
		c = 'V';
		if (symbol->st_shndx == SHN_UNDEF)
			c = 'v';
	}
	else if (symbol->st_shndx == SHN_UNDEF)
		c = 'U';
	else if (symbol->st_shndx == SHN_ABS)
		c = 'A';
	else if (symbol->st_shndx == SHN_COMMON)
		c = 'C';
	else {
		const Elf64_Shdr *sh = &shdr[symbol->st_shndx];
		
		if (sh->sh_type == SHT_NOBITS && sh->sh_flags == (SHF_ALLOC | SHF_WRITE))
			c = 'B';
		else if (sh->sh_type == SHT_PROGBITS && sh->sh_flags == SHF_ALLOC)
			c = 'R';
		else if (sh->sh_type == SHT_PROGBITS && sh->sh_flags == (SHF_ALLOC | SHF_WRITE))
			c = 'D';
		else if (sh->sh_type == SHT_PROGBITS && sh->sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
			c = 'T';
		else if (sh->sh_type == SHT_DYNAMIC)
			c = 'D';
		else
			c = ('t' - 32);
	}
	
	if (ELF64_ST_BIND(symbol->st_info) == STB_LOCAL && c != '?')
   		c += 32;
	
	return c;
}

static void elf_64_display_symbols(void *data)
{
	const Elf64_Sym *symbol = (Elf64_Sym*)data;
	const char *symbol_name = symtab_strtab + symbol->st_name;
	char type = elf_64_get_type(symbol);

	if (symbol->st_value != 0 || (symbol->st_value == 0 && type == 'T'))
		ft_fprintf(1, "%016x %c %s\n", symbol->st_value, type, symbol_name);
	else
		ft_fprintf(1, "%16c %c %s\n", ' ', type, symbol_name);
}

void elf64(const char *mapping, const char *filename)
{
	t_list *symbol_list = NULL;
	Elf64_Sym *symtab = NULL;
	size_t symtab_size;

	ehdr = (Elf64_Ehdr*)mapping;
	shdr = (void*)ehdr + ehdr->e_shoff;

	for (int i = 0; i < ehdr->e_shnum; i++) {
		if (shdr[i].sh_type == SHT_SYMTAB) {
			symtab = (void*)ehdr + shdr[i].sh_offset;
			symtab_strtab = (void*)ehdr + shdr[shdr[i].sh_link].sh_offset;
			symtab_size = shdr[i].sh_size / shdr[i].sh_entsize;
			break;
		}
	}

	if (symtab == NULL) {
		ft_fprintf(2, "ft_nm: %s: no symbol table\n", filename);
		return;
	}

	for (size_t i = 0; i < symtab_size; i++) {
		if (symtab[i].st_name != 0)
			ft_lstadd_back(&symbol_list, ft_lstnew((void*)&symtab[i]));
	}

	if (ft_lstsize(symbol_list) == 0) {
		ft_fprintf(2, "ft_nm: %s: no symbols\n", filename);
		return;
	}

	merge_sort_list(&symbol_list, elf_64_compare_symbols);
	ft_lstiter(symbol_list, elf_64_display_symbols);
	ft_lstclear(&symbol_list, NULL);
}