/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nm.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:05:49 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/03/31 15:43:28 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include "libft.h"

#include <sys/types.h> // open, fstat
#include <sys/stat.h> // open, fstat
#include <fcntl.h> // open
#include <unistd.h> // fstat, close
#include <sys/mman.h> // mmap, munmap
#include <stdio.h> // perror
#include <elf.h> // ELF header

int	main(int argc, char *argv[]) {
	struct stat		file_stat;
	const char		*filename = argc < 2 ? "a.out" : argv[1];
	void			*map_start;
	int				fd;

	if ((fd = open(filename, O_RDONLY)) == -1) {
		perror("ft_nm: open");
		return 1;
	}

	fstat(fd, &file_stat);
	if ((map_start = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		perror("ft_nm: mmap");
		return 1;
	}
	
	Elf64_Ehdr	*header = (Elf64_Ehdr*)map_start;
	Elf64_Shdr	*sections = (Elf64_Shdr*)((char*)map_start + header->e_shoff);
	Elf64_Sym	*symtab = NULL;


	for (Elf64_Half i = 0; i < header->e_shnum; i++) {
		if (sections[i].sh_type == SHT_SYMTAB) {
			symtab = (Elf64_Sym*)((char*)map_start + sections[i].sh_offset);
			break;
		}
	}

	for (;;)
		symtab += sizeof(Elf64_Sym);

	
	munmap(map_start, file_stat.st_size);
	close(fd);
	return 0;
}