/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   elf64.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/27 10:08:49 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/03 11:56:36 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include "libft.h"
#include "ft_fprintf.h"
#include <elf.h> // elf
#include <stddef.h> // NULL

static const Elf64_Ehdr	*ehdr = NULL;
static const Elf64_Shdr	*shdr = NULL;
static const char *symtab_strtab = NULL;


static inline int elf64_compare_symbols(void *a, void *b)
{
	return ft_strcmp(((t_symbol*)a)->name, ((t_symbol*)b)->name);
}

/* static char elf64_get_type_by_flag(const Elf64_Shdr *sh, bool local)
{
	if ((sh->sh_flags & SHF_WRITE) != SHF_WRITE) {
		if (sh->sh_type == SHT_GROUP)
			return (local ? 'n' : 'N');
		if (sh->sh_flags & SHF_EXECINSTR)
			return (local ? 't' : 'T');
		return (local ? 'r' : 'R');
	}
	if (sh->sh_type == SHT_NOBITS && sh->sh_flags == (SHF_ALLOC|SHF_WRITE))
		return (local ? 'b' : 'B');
	if (sh->sh_type == SHT_PROGBITS && sh->sh_type == SHT_DYNAMIC)
		return (local ? 'd' : 'D');
	
	return (char)-1;
}

static char elf64_get_type_by_section(const char *sh_name, bool local)
{
	if (!ft_strcmp(sh_name, ".bss"))
		return (local ? 'b' : 'B');
	if (!ft_strcmp(sh_name, ".sbss"))
		return (local ? 's' : 'S');
	if (!ft_strcmp(sh_name, ".debug"))
		return (local ? 'n' : 'N');
	if (!ft_strcmp(sh_name, ".data") || !ft_strcmp(sh_name, ".data1"))
		return (local ? 'd' : 'D');
	if (!ft_strcmp(sh_name, ".drectve") || !ft_strcmp(sh_name, ".idata")) //TODO: check
		return (local ? 'i' : 'I');
	if (!ft_strcmp(sh_name, ".edata"))
		return (local ? 'e' : 'E');
	if (!ft_strcmp(sh_name, ".pdata"))
		return (local ? 'p' : 'P');
	if (!ft_strcmp(sh_name, ".rodata") || !ft_strcmp(sh_name, ".rodata1"))
		return (local ? 'r' : 'R');
	if (!ft_strcmp(sh_name, ".scommon"))
		return (local ? 'c' : 'C');
	if (!ft_strcmp(sh_name, ".sdata"))
		return (local ? 'g' : 'G');
	if (!ft_strcmp(sh_name, ".code") || !ft_strcmp(sh_name, ".fini") || !ft_strcmp(sh_name, ".init") || !ft_strcmp(sh_name, ".text") || !ft_strcmp(sh_name, ".plt") || !ft_strcmp(sh_name, ".fini_array") || !ft_strcmp(sh_name, ".init_array"))
		return (local ? 't' : 'T');
	
	return (char)-1;
	
}

static char elf64_get_type_by_index(const Elf64_Sym *symbol)
{
	const unsigned char st_type = ELF64_ST_TYPE(symbol->st_info);
	const unsigned char st_bind = ELF64_ST_BIND(symbol->st_info);

	if (symbol->st_shndx == SHN_ABS)
		return (st_bind == STB_LOCAL ? 'a' : 'A');
	if (symbol->st_shndx == SHN_COMMON)
		return 'C';
	if (st_bind == STB_WEAK && st_type == STT_OBJECT)
		return (symbol->st_shndx == SHN_UNDEF ? 'v' : 'V');
	if (st_bind == STB_WEAK)
		return (symbol->st_shndx == SHN_UNDEF ? 'w' : 'W');
	if (symbol->st_shndx == SHN_UNDEF)
		return 'U';
	if (st_bind == STB_GNU_UNIQUE)
		return 'u';
	
	return (char)-1;
}

static char elf64_get_type(const Elf64_Sym *symbol)
{
	char c;

	if ((c = elf64_get_type_by_index(symbol)) != -1)
		return c;
	
	const char *sh_name = (void*)ehdr + shdr[ehdr->e_shstrndx].sh_offset + shdr[symbol->st_shndx].sh_name;
	bool local = ELF64_ST_BIND(symbol->st_info) == STB_LOCAL;
	if ((c = elf64_get_type_by_section(sh_name, local)) != -1)
		return c;
	
	const Elf64_Shdr *sh = &shdr[symbol->st_shndx];
	if ((c = elf64_get_type_by_flag(sh, local)) != -1)
		return c;
	
	return '?';
} */

static void elf64_display_symbols(void *data)
{
	const t_symbol *symbol = (t_symbol*)data;
	char type = '?';
	

	if (symbol->st_value != 0 || (symbol->st_value == 0 && type == 'T'))
		ft_fprintf(1, "%016x %c %s\n", symbol->st_value, type, symbol->name);
	else
		ft_fprintf(1, "%16c %c %s\n", ' ', type, symbol->name);
}

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
		
		custom_symbol->section.sh_name = (void*)ehdr + shdr[ehdr->e_shstrndx].sh_offset + sh->sh_name;
		custom_symbol->section.sh_type = sh->sh_type;
		custom_symbol->section.sh_flags = sh->sh_flags;
	}

	return ft_lstnew((void*)custom_symbol);
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
			ft_lstadd_back(&symbol_list, create_symbol_node(symtab + i));
	}

	if (ft_lstsize(symbol_list) == 0) {
		ft_fprintf(2, "ft_nm: %s: no symbols\n", filename);
		return;
	}

	merge_sort_list(&symbol_list, elf64_compare_symbols);
	ft_lstiter(symbol_list, elf64_display_symbols);
	ft_lstclear(&symbol_list, NULL);
}