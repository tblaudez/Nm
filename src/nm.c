/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nm.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:05:49 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/12 10:37:05 by tblaudez      ########   odam.nl         */
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
#include <stdlib.h> // exit
#include <ar.h> // ELFMAG


void identify_file(const char *mapping, size_t file_size, const char *filename)
{
	if (!ft_strncmp(mapping, ELFMAG, SELFMAG))
		elf_common(mapping, filename);
	else if (!ft_strncmp(mapping, ARMAG, SARMAG))
		archive(mapping, file_size);
	else
		ft_fprintf(2, "ft_nm: '%s': file format not recognized\n", filename);
}

void open_file(const char *filename)
{
	struct stat file_stat;
	char *mapping;
	int fd;

	if ((fd = open(filename, O_RDONLY)) == -1) {
		ft_fprintf(2, "ft_nm: '%s': Could not open\n", filename);
		return;
	}
	
	fstat(fd, &file_stat);
	if (file_stat.st_size == 0)
		return;
	
	if ((mapping = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		ft_fprintf(2, "ft_nm: '%s':Could not map file\n", filename);
		exit(EXIT_FAILURE);
	}

	identify_file(mapping, file_stat.st_size, filename);
	
	close(fd);
	munmap(mapping, file_stat.st_size);
}

int main(int argc, char **argv)
{
	if (argc == 1)
		open_file("a.out");
	else if (argc == 2)
		open_file(argv[1]);
	else {
		for (int i = 1; i < argc; i++) {
			open_file(argv[i]);
		}
	}

	return 0;
}