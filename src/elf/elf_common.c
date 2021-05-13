/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   elf_common.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/04 12:03:05 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/13 10:53:06 by tblaudez      ########   odam.nl         */
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

static inline int check_file_corruption(const t_file *file_info)
{
	const char *mapping = file_info->mapping;

	if (!(mapping[EI_CLASS] == ELFCLASS32 || mapping[EI_CLASS] == ELFCLASS64)) {
		ft_fprintf(2, "ft_nm: '%s': Invalid file class\n", file_info->name);
		return -1;
	}
	
	if (!(mapping[EI_DATA] == ELFDATA2LSB || mapping[EI_DATA] == ELFDATA2MSB)) {
		ft_fprintf(2, "ft_nm: '%s': Invalid file endianess\n", file_info->name);
		return -1;
	}
	
	if (mapping[EI_VERSION] != EV_CURRENT) {
		ft_fprintf(2, "ft_nm: '%s': Invalid file version\n", file_info->name);
		return -1;
	}

	return 0;
}

void elf_common(const t_file *file_info)
{
	g_swap_endian = (file_info->mapping[EI_DATA] == ELFDATA2MSB);
	t_list *symbol_list = NULL;

	if (check_file_corruption(file_info) == -1)
		return;

	if (file_info->mapping[EI_CLASS] == ELFCLASS32) {
		width = 8;
		if (elf32(&symbol_list, file_info) == -1)
			return;
	}
	else if (file_info->mapping[EI_CLASS] == ELFCLASS64) {
		width = 16;
		if (elf64(&symbol_list, file_info) == -1)
			return;
	}

	if (symbol_list == NULL) {
		ft_fprintf(2, "ft_nm: %s: no symbols\n", file_info->name);
		return;
	}

	merge_sort_list(&symbol_list, compare_symbols);
	ft_lstiter(symbol_list, display_symbols);
	ft_lstclear(&symbol_list, free);
}