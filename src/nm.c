/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nm.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:05:49 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/13 09:17:18 by tblaudez      ########   odam.nl         */
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


static void identify_file(t_file *file_info)
{
	if (!ft_strncmp(file_info->mapping, ELFMAG, SELFMAG))
		elf_common(file_info);
	else if (!ft_strncmp(file_info->mapping, ARMAG, SARMAG))
		archive(file_info);
	else
		ft_fprintf(2, "ft_nm: '%s': file format not recognized\n", file_info->name);
}

static void open_file(const char *filename)
{
	static t_file file_info;
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

	file_info.name = filename;
	file_info.size = file_stat.st_size;
	file_info.mapping = mapping;

	identify_file(&file_info);
	
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