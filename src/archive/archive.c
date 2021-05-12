/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   archive.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/10 08:49:03 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/12 10:42:12 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"
#include "libft.h"
#include "nm.h"
#include <ar.h> // ELFMAG


static void get_next_header(struct ar_hdr **aptr, void *endptr)
{
	*aptr = (void*)*aptr + sizeof(**aptr) + ft_strtol((*aptr)->ar_size, NULL, 10);
	if ((void*)*aptr >= endptr)
		*aptr = NULL;
}


void archive(const char *mapping, size_t file_size)
{
	struct ar_hdr *arhdr = (struct ar_hdr*)(mapping + SARMAG);
	void *endptr = (void*)mapping + file_size;
	const char *arstrtab = NULL;
	static char namebuf[64];


	while (arhdr) {
		ft_memset(namebuf, 0, sizeof(namebuf));
		
		// Ignore lookup table
		if (!ft_strncmp(arhdr->ar_name, "/ ", 2)) {
			get_next_header(&arhdr, endptr);
			continue;
		}
		// Save string table
		if (!ft_strncmp(arhdr->ar_name, "//", 2)) {
			arstrtab = (void*)arhdr + sizeof(*arhdr);
			get_next_header(&arhdr, endptr);
			continue;
		}
		// Object name in string table
		if (arhdr->ar_name[0] == '/') {
			const char *name = arstrtab + ft_strtol(arhdr->ar_name + 1, NULL, 10);
			size_t size = (size_t)(ft_strchr(name, '/') - name);
			ft_strncpy(namebuf, name, size);
		}
		// Object name in header
		else {
			size_t size = (size_t)(ft_strchr(arhdr->ar_name, '/') - arhdr->ar_name);
			ft_strncpy(namebuf, arhdr->ar_name, size);
		}

		#ifdef NM_COLORS
		ft_fprintf(1, "\n{UNDERLINED}%s:{EOC}\n", namebuf);
		#else
		ft_fprintf(1, "\n%s:\n", namebuf);
		#endif
		elf_common((void*)arhdr + sizeof(*arhdr), namebuf);
		get_next_header(&arhdr, endptr);
	}
}