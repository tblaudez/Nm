/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   elf_common.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/04 12:03:05 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/06 08:38:18 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include "libft.h"
#include "ft_fprintf.h"
#include <elf.h> // elf
#include <stdlib.h> // free

bool g_swap_endian = false;
static int width = 16;

static inline int compare_symbols(void *a, void *b)
{
	return ft_strcmp(((t_symbol*)a)->name, ((t_symbol*)b)->name);
}

static char get_type_by_flag(const t_symbol *symbol)
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

static char get_type_by_section(const t_symbol *symbol)
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

static char get_type_by_index(const t_symbol *symbol)
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

static char get_type(const t_symbol *symbol)
{
	char c;

	if ((c = get_type_by_index(symbol)) != '?')
		return c;
	
	if ((c = get_type_by_section(symbol)) != '?')
		return c;
	
	if ((c = get_type_by_flag(symbol)) != '?')
		return c;
	
	return c;
} 

static void display_symbols(void *data)
{
	const t_symbol *symbol = (t_symbol*)data;
	char type;

	if (symbol->st_type == STT_FILE)
		return;
	
	type = get_type(symbol);

	if (ft_strchr("vwU", type))
		ft_fprintf(1, "%*c %c %s\n", width, ' ', type, symbol->name);
	else if (symbol->st_value != 0 || (symbol->st_value == 0 && ft_strchr("ATnbaDRWBdtru", type)))
		ft_fprintf(1, "%0*x %c %s\n", width, symbol->st_value, type, symbol->name);
	else
		ft_fprintf(1, "%*c %c %s\n", width, ' ', type, symbol->name);
}

void elf_common(const char *mapping, const char *filename)
{
	g_swap_endian = (mapping[EI_DATA] == ELFDATA2MSB);
	t_list *symbol_list = NULL;

	if (mapping[EI_CLASS] == ELFCLASS32) {
		symbol_list = elf32(mapping);
		width = 8;
	}
	else if (mapping[EI_CLASS] == ELFCLASS64) {
		symbol_list = elf64(mapping);
		width = 16;
	}

	if (symbol_list == NULL) {
		ft_fprintf(2, "ft_nm: %s: no symbols\n", filename);
		return;
	}

	merge_sort_list(&symbol_list, compare_symbols);
	ft_lstiter(symbol_list, display_symbols);
	ft_lstclear(&symbol_list, free);
}