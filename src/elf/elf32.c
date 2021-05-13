/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   elf32.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/03 08:35:31 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/13 11:24:38 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include "libft.h"
#include "ft_fprintf.h"
#include <elf.h> // Elf32_Ehdr, Elf32_Shdr
#include <stddef.h> // NULL

extern bool g_swap_endian;

static t_list *create_symbol_node(const Elf32_Sym *symbol, const char *symstrtab, \
	const Elf32_Shdr *shdr, const char *shstrtab)
{
	t_symbol *custom_symbol = ft_memalloc(sizeof(t_symbol));

	custom_symbol->name = symstrtab + SWAP32(symbol->st_name);
	custom_symbol->st_type = ELF32_ST_TYPE(SWAP8(symbol->st_info));
	custom_symbol->st_bind = ELF32_ST_BIND(SWAP8(symbol->st_info));
	custom_symbol->st_shndx = SWAP16(symbol->st_shndx);
	custom_symbol->st_value = SWAP32(symbol->st_value);

	// Edge case for Common symbols
	if (SWAP16(symbol->st_shndx) == SHN_COMMON)
		custom_symbol->st_value = SWAP32(symbol->st_size);

	if (!(custom_symbol->st_shndx >= SHN_LORESERVE && custom_symbol->st_shndx <= SHN_HIRESERVE)) {
		const Elf32_Shdr *sh = &shdr[custom_symbol->st_shndx];
		custom_symbol->section.name = shstrtab + SWAP32(sh->sh_name);
		custom_symbol->section.sh_type = SWAP32(sh->sh_type);
		custom_symbol->section.sh_flags = SWAP32(sh->sh_flags);
	}

	return ft_lstnew((void*)custom_symbol);
}

static inline int check_file_corruption(const Elf32_Ehdr *ehdr, const t_file *file_info)
{
	// Offset too big or too little
	if (SWAP32(ehdr->e_shoff) >= (file_info->size - sizeof(Elf32_Shdr)) \
		|| SWAP32(ehdr->e_shoff) < (sizeof(Elf32_Ehdr) + sizeof(Elf32_Phdr)))
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

int elf32(t_list **alist, const t_file *file_info)
{
	const Elf32_Ehdr *ehdr = (Elf32_Ehdr*)file_info->mapping;
	
	if (check_file_corruption(ehdr, file_info) == -1)
		return -1;

	const Elf32_Shdr *shdr = (void*)ehdr + SWAP32(ehdr->e_shoff);
	const char *shstrtab = (void*)ehdr + SWAP32(shdr[SWAP16(ehdr->e_shstrndx)].sh_offset);

	Elf32_Sym *symtab = NULL;
	size_t symtab_size = 0;
	const char *symstrtab = NULL;
	
	for (int i = 0; i < SWAP16(ehdr->e_shnum); i++) {
		if (SWAP32(shdr[i].sh_type) == SHT_SYMTAB) {
			symtab = (void*)ehdr + SWAP32(shdr[i].sh_offset);
			symstrtab = (void*)ehdr + SWAP32(shdr[SWAP32(shdr[i].sh_link)].sh_offset);
			symtab_size = SWAP32(shdr[i].sh_size) / SWAP32(shdr[i].sh_entsize);
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