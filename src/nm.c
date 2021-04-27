/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nm.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:05:49 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/27 11:26:30 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include "libft.h"
#include "ft_fprintf.h"

#include <sys/types.h> // open, fstat
#include <sys/stat.h> // open, fstat
#include <fcntl.h> // open
#include <unistd.h> // fstat, close
#include <sys/mman.h> // mmap, munmap
#include <elf.h> // ELFMAG, SELFMAG, EI_CLASS


void ft_nm(const char *filename)
{
	struct stat file_stat;
	char *mapping;
	int fd;

	if ((fd = open(filename, O_RDONLY)) == -1) {
		ft_fprintf(STDERR_FILENO, "ft_nm: open: Could not open '%s'\n", filename);
		return;
	}
	fstat(fd, &file_stat);
	if ((mapping = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		ft_fprintf(STDERR_FILENO, "ft_nm: mmap: Could not map '%s'\n", filename);
		close(fd);
		return;
	}

	if (!ft_strncmp(mapping, ELFMAG, SELFMAG)) {
		if (mapping[EI_CLASS] == ELFCLASS32)
			elf_32(mapping);
		else if (mapping[EI_CLASS] == ELFCLASS64)
			elf_64(mapping);
	}
	
	close(fd);
	munmap(mapping, file_stat.st_size);
}

int main(int argc, char **argv)
{
	if (argc == 1)
		ft_nm("a.out");
	else if (argc == 2)
		ft_nm(argv[1]);
	else for (int i = 1; i < argc; i++) {
		ft_fprintf(STDOUT_FILENO, "\n%s:\n", argv[1]);
		ft_nm(argv[1]);
	}

	return 0;
}