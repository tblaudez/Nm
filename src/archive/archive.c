/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   archive.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/10 08:49:03 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/11 10:15:09 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"
#include "libft.h"
#include <ar.h> // ELFMAG


void archive(const char *mapping, const char *filename)
{
	struct ar_hdr *arhdr = (struct ar_hdr*)(mapping + SARMAG);
	const char *arstrtab = NULL;
	char file_name[64];
	size_t offset, size;

	while (arhdr) {
		if (arhdr->ar_name[0] == '/') {
			switch (arhdr->ar_name[1]) {
				case ' ':
					break;
				
				case '/':
					arstrtab = (void*)arhdr + sizeof(*arhdr);
					break;
				
				default:
					offset = ft_strtol(arhdr->ar_name + 1, NULL, 10);
					size = ft_strchr(arstrtab + offset, '/') - (arstrtab + offset);
					ft_strncpy(file_name, arstrtab + offset, size);
					break;
			}
		}
		else {
			size = ft_strnchr(arhdr->ar_name, '/', sizeof(arhdr->ar_name)) - arhdr->ar_name;
			ft_strncpy(file_name, arhdr->ar_name, size);
		}

		arhdr = (void*)arhdr + sizeof(*arhdr) + ft_strtol(arhdr->ar_size, NULL, 10);
	}
}