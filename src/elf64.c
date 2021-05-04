/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   elf64.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/27 10:08:49 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/04 12:20:52 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include "libft.h"
#include "ft_fprintf.h"
#include <elf.h> // Elf64_Ehdr, Elf64_Shdr
#include <stddef.h> // NULL
#include <stdlib.h> // free

static const Elf64_Ehdr *ehdr = NULL;
static const Elf64_Shdr *shdr = NULL;
static const char *symtab_strtab = NULL;
static const char *shdr_strtab = NULL;


static t_list *create_symbol_node(const Elf64_Sym *symbol)
{
	t_symbol *custom_symbol = ft_memalloc(sizeof(t_symbol));

	custom_symbol->name = symtab_strtab + symbol->st_name;
	custom_symbol->st_type = ELF64_ST_TYPE(symbol->st_info);
	custom_symbol->st_bind = ELF64_ST_BIND(symbol->st_info);
	custom_symbol->st_shndx = symbol->st_shndx;
	custom_symbol->st_value = symbol->st_value;

	if (!(symbol->st_shndx >= SHN_LORESERVE && symbol->st_shndx <= SHN_HIRESERVE)) {
		const Elf64_Shdr *sh = &shdr[symbol->st_shndx];
		
		custom_symbol->section.sh_name = shdr_strtab + sh->sh_name;
		custom_symbol->section.sh_type = sh->sh_type;
		custom_symbol->section.sh_flags = sh->sh_flags;
	}

	return ft_lstnew((void*)custom_symbol);
}

t_list *elf64(const char *mapping)
{
	ehdr = (Elf64_Ehdr*)mapping;
	shdr = (void*)ehdr + ehdr->e_shoff;
	shdr_strtab = (void*)ehdr + shdr[ehdr->e_shstrndx].sh_offset;

	Elf64_Sym *symtab = NULL;
	size_t symtab_size = 0;
	for (int i = 0; i < ehdr->e_shnum; i++) {
		if (shdr[i].sh_type == SHT_SYMTAB) {
			symtab = (void*)ehdr + shdr[i].sh_offset;
			symtab_strtab = (void*)ehdr + shdr[shdr[i].sh_link].sh_offset;
			symtab_size = shdr[i].sh_size / shdr[i].sh_entsize;
			break;
		}
	}

	t_list *symbol_list = NULL;
	for (size_t i = 0; i < symtab_size; i++) {
		if (symtab[i].st_name != 0)
			ft_lstadd_back(&symbol_list, create_symbol_node(symtab + i));
	}

	return symbol_list;
}