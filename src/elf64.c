/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   elf64.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/27 10:08:49 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/29 11:58:46 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_fprintf.h"
#include <elf.h> // elf
#include <stddef.h> // NULL

// G g - Initialized data section for small objects
// I - Symbol is an indirect reference to another symbol
// P - Symbols is in a stack unwind section
// S - Symbol is in uninitialized or zero-initialized data section for small objects

static const Elf64_Ehdr *ehdr = NULL;
static const Elf64_Shdr *shdr = NULL;
static const char *symtab_strtab = NULL;
static const char *shdr_strtab = NULL;

static inline int compare_symbols(void *a, void *b)
{
	return ft_strcmp(symtab_strtab + ((Elf64_Sym*)a)->st_name, symtab_strtab + ((Elf64_Sym*)b)->st_name);
}

static char get_symbol_letter(const Elf64_Sym *symbol)
{
	// File -> ignored
	if (ELF64_ST_TYPE(symbol->st_info) == STT_FILE)
		return -1;
	// Indirect function
	if (ELF64_ST_TYPE(symbol->st_info) == STT_GNU_IFUNC)
		return 'i';
	// Unique global symbol
	if (ELF64_ST_BIND(symbol->st_info) == STB_GNU_UNIQUE)
		return 'u';
	// Weak object
	if (ELF64_ST_BIND(symbol->st_info) == STB_WEAK && ELF64_ST_TYPE(symbol->st_info) == STT_OBJECT)
		return (symbol->st_shndx == SHN_UNDEF ? 'v' : 'V');
	// Weak symbol
	if (ELF64_ST_BIND(symbol->st_info) == STB_WEAK)
		return (symbol->st_shndx == SHN_UNDEF ? 'w' : 'W');
	// Absolute symbol
	if (symbol->st_shndx == SHN_ABS)
		return 'A';
	// Common symbol
	if (symbol->st_shndx == SHN_COMMON)
		return 'C';
	// Undefined symbol
	if (symbol->st_shndx == SHN_UNDEF)
		return 'U';
	
	const Elf64_Shdr *sh = &shdr[symbol->st_shndx];

	// .bss - NOBITS WA
	if (sh->sh_type == SHT_NOBITS && sh->sh_flags == (SHF_WRITE|SHF_ALLOC))
		return (ELF64_ST_BIND(symbol->st_info) == STB_LOCAL ? 'b' : 'B');
	
	// .data section - PROGBITS WA
	if ((sh->sh_type == SHT_PROGBITS && sh->sh_flags == (SHF_WRITE|SHF_ALLOC)) || sh->sh_type == SHT_DYNAMIC)
		return (ELF64_ST_BIND(symbol->st_info) == STB_LOCAL ? 'd' : 'D');
	
	// .rodata - PROGBITS A
	if (sh->sh_type == SHT_PROGBITS && sh->sh_flags == SHF_ALLOC)
		return (ELF64_ST_BIND(symbol->st_info) == STB_LOCAL ? 'r' : 'R');
	
	// .text - PROGBITS AX
	if ((sh->sh_type == SHT_PROGBITS && sh->sh_flags == (SHF_ALLOC|SHF_EXECINSTR)) || sh->sh_type == SHT_FINI_ARRAY || sh->sh_type == SHT_INIT_ARRAY)
		return (ELF64_ST_BIND(symbol->st_info) == STB_LOCAL ? 't' : 'T');
	
	// Debugging symbol
	if (sh->sh_type == SHT_PROGBITS && sh->sh_flags == 0)
		return (ELF64_ST_BIND(symbol->st_info) == STB_LOCAL ? 'n' : 'N');
	
	return '?';
}

static void display_symbols(void *data)
{
	const Elf64_Sym *symbol = (Elf64_Sym*)data;
	const char *symbol_name = symtab_strtab + symbol->st_name;
	char c;

	if ((c = get_symbol_letter(symbol)) == -1)
		return;
	
	if (ft_strchr("Uwv", c))
		ft_fprintf(1, "%16c ", ' ');
	else
		ft_fprintf(1, "%016x ", symbol->st_value);
	
	ft_fprintf(1, "%c %s\n", c, symbol_name);
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