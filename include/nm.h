/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nm.h                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:05:42 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/26 10:36:30 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <elf.h> // Elf64_Sym

typedef struct s_symbol {
	const Elf64_Sym *symbol;
	const char *name;
} t_symbol;


int compare_symbols(void *a, void *b);
t_symbol *create_symbol(const Elf64_Sym *symbol, const char *name);