/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nm.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:05:49 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/04 12:05:51 by tblaudez      ########   odam.nl         */
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
#include <errno.h>
#include <stdio.h> // perror
#include <stdlib.h> // exit


void ft_nm(const char *filename)
{
	struct stat file_stat;
	char *mapping;
	int fd;

	if ((fd = open(filename, O_RDONLY)) == -1) {
		perror("ft_nm");
		return;
	}
	fstat(fd, &file_stat);
	if (file_stat.st_size == 0)
		return;
	if ((mapping = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		perror("ft_nm");
		close(fd);
		exit(EXIT_FAILURE);
	}

	if (!ft_strncmp(mapping, ELFMAG, SELFMAG))
		elf(mapping, filename);
	else ft_fprintf(2, "ft_nm: %s: file format not recognized\n", filename);
	
	close(fd);
	munmap(mapping, file_stat.st_size);
}

int main(int argc, char **argv)
{
	if (argc == 1)
		ft_nm("a.out");
	else if (argc == 2)
		ft_nm(argv[1]);
	else {
		for (int i = 1; i < argc; i++) {
			ft_nm(argv[i]);
		}
	}

	return 0;
}