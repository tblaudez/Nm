/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_type.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/12 11:01:40 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/13 11:24:49 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include "libft.h"
#include <elf.h> // elf

char get_type_by_flag(const t_symbol *symbol)
{
	const bool local = (symbol->st_bind == STB_LOCAL);

	if (symbol->section.sh_type == SHT_PREINIT_ARRAY)
		return (local ? 'd' : 'D');
	if (symbol->section.sh_type == SHT_PROGBITS && symbol->section.sh_flags == 0)
		return (local ? 'n' : 'N');
	if (symbol->section.sh_type == SHT_PROGBITS && symbol->section.sh_flags == SHF_ALLOC)
		return (local ? 'r' : 'R');
	if (symbol->section.sh_flags & SHF_EXECINSTR)
		return (local ? 't' : 'T');
	if (symbol->section.sh_type == SHT_NOBITS && symbol->section.sh_flags & SHF_ALLOC)
		return (local ? 'b' : 'B');
	if (symbol->section.sh_type == SHT_PROGBITS || symbol->section.sh_type == SHT_DYNAMIC)
		return (local ? 'd' : 'D');
	
	if ((symbol->section.sh_flags & SHF_WRITE) != SHF_WRITE) {
		if (symbol->section.sh_type == SHT_GROUP)
			return (local ? 'n' : 'N');
		
		return (local ? 'r' : 'R');
	}

	return '?';
}

char get_type_by_section(const t_symbol *symbol)
{
	const bool local = (symbol->st_bind == STB_LOCAL);
	const char *sh_name = symbol->section.name;

	if (!ft_strcmp(sh_name, ".bss"))
		return (local ? 'b' : 'B');
	if (!ft_strcmp(sh_name, ".sbss"))
		return (local ? 's' : 'S');
	if (!ft_strcmp(sh_name, ".debug"))
		return (local ? 'n' : 'N');
	if (!ft_strncmp(sh_name, ".data", 5)) // .data*
		return (local ? 'd' : 'D');
	if (!ft_strcmp(sh_name, ".drectve") || !ft_strcmp(sh_name, ".idata"))
		return (local ? 'i' : 'I');
	if (!ft_strcmp(sh_name, ".edata"))
		return (local ? 'e' : 'E');
	if (!ft_strcmp(sh_name, ".pdata"))
		return (local ? 'p' : 'P');
	if (!ft_strncmp(sh_name, ".rodata", 7)) // .rodata*
		return (local ? 'r' : 'R');
	if (!ft_strcmp(sh_name, ".scommon"))
		return (local ? 'c' : 'C');
	if (!ft_strcmp(sh_name, ".sdata"))
		return (local ? 'g' : 'G');
	if (!ft_strcmp(sh_name, ".code") || !ft_strcmp(sh_name, ".fini") || !ft_strcmp(sh_name, ".init") \
		|| !ft_strcmp(sh_name, ".text") || !ft_strcmp(sh_name, ".plt") || !ft_strcmp(sh_name, ".fini_array") \
		|| !ft_strcmp(sh_name, ".init_array"))
		return (local ? 't' : 'T');
	
	return '?';
	
}

char get_type_by_index(const t_symbol *symbol)
{
	if (symbol->st_shndx == SHN_ABS)
		return (symbol->st_bind == STB_LOCAL ? 'a' : 'A');
	if (symbol->st_shndx == SHN_COMMON)
		return 'C';
	if (symbol->st_bind == STB_WEAK && symbol->st_type == STT_OBJECT)
		return (symbol->st_shndx == SHN_UNDEF ? 'v' : 'V');
	if (symbol->st_type == STT_GNU_IFUNC)
		return 'i';
	if (symbol->st_bind == STB_WEAK)
		return (symbol->st_shndx == SHN_UNDEF ? 'w' : 'W');
	if (symbol->st_shndx == SHN_UNDEF)
		return 'U';
	if (symbol->st_bind == STB_GNU_UNIQUE)
		return 'u';
	
	return '?';
}

char get_type(const t_symbol *symbol)
{
	char c;

	if ((c = get_type_by_index(symbol)) == '?')
		if ((c = get_type_by_section(symbol)) == '?')
			if ((c = get_type_by_flag(symbol)) == '?')
				return '?';
	
	return c;
} 
