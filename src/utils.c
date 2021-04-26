/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/26 10:34:47 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/26 10:37:20 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h> // Elf64_Sym
#include "nm.h" // t_symbol
#include "libft.h" // strcmp, memalloc


int compare_symbols(void *a, void *b)
{
	return ft_strcmp(((t_symbol*)a)->name, ((t_symbol*)b)->name);
}

t_symbol *create_symbol(const Elf64_Sym *symbol, const char *name)
{
	t_symbol *custom_symbol = (t_symbol*)ft_memalloc(sizeof(t_symbol));

	custom_symbol->symbol = symbol;
	custom_symbol->name = name;

	return custom_symbol;
}