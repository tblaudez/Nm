/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   elf_common.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/04 12:03:05 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/12 11:02:15 by tblaudez      ########   odam.nl         */
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

static void display_symbols(void *data)
{
	const t_symbol *symbol = (t_symbol*)data;
	char type;

	if (symbol->st_type == STT_FILE)
		return;
	
	type = get_type(symbol);

	if ((symbol->st_value != 0 && !ft_strchr("vwU", type)) || ft_strchr("ATnbaDRWBdtruVi", type)) {
		#ifdef NM_COLORS
			ft_fprintf(1, "{BLUE}%0*x {YELLOW}%c {GREEN}%s{EOC}\n", width, symbol->st_value, type, symbol->name);
		#else
			ft_fprintf(1, "%0*x %c %s\n", width, symbol->st_value, type, symbol->name);
		#endif
	}
	else {
		#ifdef NM_COLORS
			ft_fprintf(1, "{BLUE}%*s {YELLOW}%c {GREEN}%s{EOC}\n", width, " ", type, symbol->name);
		#else
			ft_fprintf(1, "%*s %c %s\n", width, " ", type, symbol->name);
		#endif
	}
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