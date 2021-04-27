/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   elf64.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/27 10:08:49 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/27 11:44:59 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_fprintf.h"
#include <elf.h> // elf
#include <stddef.h> // NULL

// G g - Initialized data section for small objects
// i - Symbol is an indirect function
// I - Symbol is an indirect reference to another symbol
// P - Symbols is in a stack unwind section
// S - Symbol is in uninitialized or zero-initialized data section for small objects

const Elf64_Ehdr *ehdr = NULL;
const Elf64_Shdr *shdr = NULL;
const char *symtab_strtab = NULL;
const char *shdr_strtab = NULL;

static inline int compare_symbols(void *a, void *b)
{
	return ft_strcmp(symtab_strtab + ((Elf64_Sym*)a)->st_name, symtab_strtab + ((Elf64_Sym*)b)->st_name);
}

static char get_symbol_letter(const Elf64_Sym *symbol)
{
	// Unique global symbol
	if (ELF64_ST_BIND(symbol->st_info) == STB_GNU_UNIQUE)
		return 'u';
	// Weak object
	else if (ELF64_ST_BIND(symbol->st_info) == STB_WEAK && ELF64_ST_TYPE(symbol->st_info) == STT_OBJECT)
		return (symbol->st_shndx == SHN_UNDEF ? 'v' : 'V');
	// Weak symbol
	else if (ELF64_ST_BIND(symbol->st_info) == STB_WEAK)
		return (symbol->st_shndx == SHN_UNDEF ? 'w' : 'W');
	// Absolute symbol
	else if (symbol->st_shndx == SHN_ABS)
		return 'A';
	// Common symbol
	else if (symbol->st_shndx == SHN_COMMON)
		return 'C';
	// Undefined symbol
	else if (symbol->st_shndx == SHN_UNDEF)
		return 'U';
	
	const bool local_symbol = (ELF64_ST_BIND(symbol->st_info) == STB_LOCAL);
	const Elf64_Shdr *sh = &shdr[symbol->st_shndx];
	const char *sh_name = shdr_strtab + sh->sh_name;
	const char *spc_sect[][2] = {{".rodata", "r"}, {".rodata1", "r"}, {".data", "d"}, {".data1", "d"}, \
		{".text", "t"}, {".bss", "b"}, {".init", "t"}, {".init_array", "t"}, {".fini", "t"}, \
		{".fini_array", "t"}, {".dynamic", "b"}};
	
	for (size_t i = 0; i < sizeof(spc_sect) / sizeof(*spc_sect); i++) {
		if (!ft_strcmp(spc_sect[i][0], sh_name))
			return (local_symbol ? *spc_sect[i][1] : *spc_sect[i][1] - 32);
	}

	// Read only data section
	if ((sh->sh_type == SHT_PROGBITS && sh->sh_flags == SHF_ALLOC))
		return (local_symbol ? 'r' : 'R');
	// Initialized data section
	if ((sh->sh_type == SHT_PROGBITS && sh->sh_flags == (SHF_WRITE|SHF_ALLOC)))
		return (local_symbol ? 'd' : 'D');
	// Debugging symbol
	if ((sh->sh_type == SHT_PROGBITS && sh->sh_flags == 0))
		return 'N';
	// Text section
	if ((sh->sh_type == SHT_PROGBITS && sh->sh_flags == (SHF_WRITE|SHF_EXECINSTR)))
		return (local_symbol ? 't' : 'T');
	// BSS data section
	if ((sh->sh_type == SHT_NOBITS && sh->sh_flags & SHF_ALLOC))
		return (local_symbol ? 'b' : 'B');
	
	return '?';
}

static void display_symbols(void *data)
{
	const Elf64_Sym *symbol = (Elf64_Sym*)data;
	const char *symbol_name = symtab_strtab + symbol->st_name;
	char c = get_symbol_letter(symbol);

	if (ft_strchr("Uwv", c))
		ft_fprintf(1, "%16c %c %s\n", ' ', c, symbol_name);
	else if (symbol->st_value != 0)
		ft_fprintf(1, "%016x %c %s\n", symbol->st_value, c, symbol_name);
}

void elf_64(const char *mapping)
{
	t_list *symbol_list = NULL;

	ehdr = (Elf64_Ehdr*)mapping;
	shdr = (void*)ehdr + ehdr->e_shoff;
	shdr_strtab = (void*)ehdr + shdr[ehdr->e_shstrndx].sh_offset;

	for (int i = 0; i < ehdr->e_shnum; i++) {
		if (shdr[i].sh_type == SHT_SYMTAB) {
			symtab_strtab = (void*)ehdr + shdr[shdr[i].sh_link].sh_offset;
			const Elf64_Sym *symtab = (void*)ehdr + shdr[i].sh_offset;
			for (size_t j = 0; j < shdr[i].sh_size / shdr[i].sh_entsize; j++) {
				if (ft_strcmp(symtab_strtab + symtab[j].st_name, "") != 0)
					ft_lstadd_back(&symbol_list, ft_lstnew((void*)&symtab[j]));
			}
			break;
		}
	}

	merge_sort_list(&symbol_list, compare_symbols);
	ft_lstiter(symbol_list, display_symbols);
	ft_lstclear(&symbol_list, NULL);
}