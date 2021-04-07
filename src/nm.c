/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nm.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:05:49 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/07 10:40:48 by anonymous     ########   odam.nl         */
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
#include <stdbool.h> // bool
#include <stdlib.h> // system TODO: remove

int	main(int argc, char *argv[]) {
	struct stat		file_stat;
	const char		*filename = argc < 2 ? "a.out" : argv[1];
	char			*mapping;
	int				fd;

	if ((fd = open(filename, O_RDONLY)) == -1) {
		perror("ft_nm: open");
		return 1;
	}

	fstat(fd, &file_stat);
	if (S_ISDIR(file_stat.st_mode)) {
		ft_putendl_fd(STDERR_FILENO, "ft_nm: Warning: is a directory"); //TODO: printf
		return 1;
	}
	
	if ((mapping = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		perror("ft_nm: mmap");
		return 1;
	}

	if (!ft_strncmp(ELFMAG, mapping, 4)) {
		ft_putendl_fd(STDERR_FILENO, "ft_nm: file format not recognized"); //TODO: printf
		return 1;
	}

	
	// Elf64_Ehdr	*header = (Elf64_Ehdr*)mapping;
	// Elf64_Shdr	*sections = (Elf64_Shdr*)((char*)mapping + header->e_shoff);
	// Elf64_Sym	*symtab = NULL;

	// for (Elf64_Half i = 0; i < header->e_shnum; i++) {
	// 	if (sections[i].sh_type == SHT_SYMTAB) {
	// 		symtab = (Elf64_Sym*)((char*)mapping + sections[i].sh_offset);
	// 		break;
	// 	}
	// }

	// for (;;)
	// 	symtab += sizeof(Elf64_Sym);

	munmap(mapping, file_stat.st_size);
	close(fd);
	return 0;
}