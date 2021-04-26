# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: tblaudez <tblaudez@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2021/03/31 15:06:14 by tblaudez      #+#    #+#                  #
#    Updated: 2021/04/26 11:21:30 by tblaudez      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

TARGET = ft_nm
CC = clang
CFLAGS = -Wall -Wextra -Werror -I include/ -I libft/include/ $(EXTRA_FLAGS)
LIBRARIES = -Llibft/ -lft

SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard include/*.h)
OBJECTS = $(SOURCES:%.c=%.o)

LIBFT = libft/libft.a

default: $(TARGET)
all: default

$(LIBFT):
	@$(MAKE) -C libft/

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(LIBFT) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBRARIES) -o $@

clean:
	-rm -f $(OBJECTS)
	@$(MAKE) -C libft/ clean

fclean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)
	@$(MAKE) -C libft/ fclean

re: fclean all

.PHONY: default all clean fclean re
