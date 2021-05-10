/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   archive.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/10 08:49:03 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/10 10:54:39 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"
#include "libft.h"
#include <ar.h> // ELFMAG
#include <unistd.h> // write

void get_next_file(struct ar_hdr **aptr)
{
	*aptr = (void*)(*aptr) + sizeof(struct ar_hdr) + ft_strtol((*aptr)->ar_size, NULL, 10);
}

void archive(const char *mapping, const char *filename)
{
	struct ar_hdr *arhdr = (struct ar_hdr*)(mapping + SARMAG);
	const char *arstrtab = NULL;
	char file_name[64];

	while (arhdr) {
		// Lookup table
		if (!ft_strncmp(arhdr->ar_name, "/ ", 2)) {
			; // Do nothing
		}
		// String table
		else if (!ft_strncmp(arhdr->ar_name, "//", 2)) {
			arstrtab = (void*)arhdr + sizeof(*arhdr);
		}
		// Filename greater than 16 characters
		else if (!ft_strncmp(arhdr->ar_name, "/", 1)) {
			uint64_t offset = ft_strtol(arhdr->ar_name + 1, NULL, 10);
			uintptr_t size = ft_strchr(arstrtab + offset, '/') - (arstrtab + offset);
			ft_strncpy(file_name, arstrtab + offset, size);
		}
		// Filename with 16 characters or less
		else {
			uintptr_t size = ft_strnchr(arhdr->ar_name, '/', sizeof(arhdr->ar_name)) - arhdr->ar_name;
			ft_strncpy(file_name, arhdr->ar_name, size);
		}

		arhdr = (void*)arhdr + sizeof(*arhdr) + ft_strtol(arhdr->ar_size, NULL, 10);
	}
}