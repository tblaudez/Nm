/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   elf64.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/27 10:08:49 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/13 11:24:43 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include "libft.h"
#include "ft_fprintf.h"
#include <elf.h> // Elf64_Ehdr, Elf64_Shdr
#include <stddef.h> // NULL

extern bool g_swap_endian;

static t_list *create_symbol_node(const Elf64_Sym *symbol, const char *symstrtab, \
	const Elf64_Shdr *shdr, const char *shstrtab)
{
	t_symbol *custom_symbol = ft_memalloc(sizeof(t_symbol));

	custom_symbol->name = symstrtab + SWAP32(symbol->st_name);
	custom_symbol->st_type = ELF64_ST_TYPE(SWAP8(symbol->st_info));
	custom_symbol->st_bind = ELF64_ST_BIND(SWAP8(symbol->st_info));
	custom_symbol->st_shndx = SWAP16(symbol->st_shndx);
	custom_symbol->st_value = SWAP64(symbol->st_value);

	// Edge case for Common symbols
	if (SWAP16(symbol->st_shndx) == SHN_COMMON)
		custom_symbol->st_value = SWAP64(symbol->st_size);

	if (!(custom_symbol->st_shndx >= SHN_LORESERVE && custom_symbol->st_shndx <= SHN_HIRESERVE)) {
		const Elf64_Shdr *sh = &shdr[custom_symbol->st_shndx];
		custom_symbol->section.name = shstrtab + SWAP32(sh->sh_name);
		custom_symbol->section.sh_type = SWAP32(sh->sh_type);
		custom_symbol->section.sh_flags = SWAP64(sh->sh_flags);
	}

	return ft_lstnew((void*)custom_symbol);
}

static inline int check_file_corruption(const Elf64_Ehdr *ehdr, const t_file *file_info)
{
	// Offset too big or too little
	if (SWAP64(ehdr->e_shoff) >= (file_info->size - sizeof(Elf64_Shdr)) \
		|| SWAP64(ehdr->e_shoff) < (sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr)))
	{
		ft_fprintf(2, "ft_nm: '%s': Invalid section header table offset\n", file_info->name);
		return -1;
	}
	// No sections
	if (SWAP16(ehdr->e_shnum) == 0) {
		ft_fprintf(2, "ft_nm: '%s': Invalid section header table entry count\n", file_info->name);
		return -1;
	}
	// Invalid type
	if (!(SWAP16(ehdr->e_type) >= 1 && SWAP16(ehdr->e_type) <= 4)) {
		ft_fprintf(2, "ft_nm: '%s': Invalid file type\n", file_info->name);
		return -1;
	}

	return 0;
}

int elf64(t_list **alist, const t_file *file_info)
{
	const Elf64_Ehdr *ehdr = (Elf64_Ehdr*)file_info->mapping;
	
	if (check_file_corruption(ehdr, file_info) == -1)
		return -1;

	const Elf64_Shdr *shdr = (void*)ehdr + SWAP64(ehdr->e_shoff);
	const char *shstrtab = (void*)ehdr + SWAP64(shdr[SWAP16(ehdr->e_shstrndx)].sh_offset);

	Elf64_Sym *symtab = NULL;
	size_t symtab_size = 0;
	const char *symstrtab = NULL;
	
	for (int i = 0; i < SWAP16(ehdr->e_shnum); i++) {
		if (SWAP32(shdr[i].sh_type) == SHT_SYMTAB) {
			symtab = (void*)ehdr + SWAP64(shdr[i].sh_offset);
			symstrtab = (void*)ehdr + SWAP64(shdr[SWAP32(shdr[i].sh_link)].sh_offset);
			symtab_size = SWAP64(shdr[i].sh_size) / SWAP64(shdr[i].sh_entsize);
			break;
		}
	}

	if (!symtab || !symstrtab || !symtab_size)
		return 0;

	for (size_t i = 0; i < symtab_size; i++) {
		if (SWAP32(symtab[i].st_name) != 0)
			ft_lstadd_back(alist, create_symbol_node(symtab + i, symstrtab, shdr, shstrtab));
	}

	return 0;
}