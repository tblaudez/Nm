# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: tblaudez <tblaudez@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2021/03/31 15:28:54 by tblaudez      #+#    #+#                  #
#    Updated: 2021/04/19 13:28:43 by tblaudez      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

TARGET = libft.a
CC = clang
CFLAGS = -Wall -Wextra -Werror -I include/ $(EXTRA_FLAGS)

SOURCES = $(wildcard src/*.c)
SOURCES += $(wildcard src/ft_fprintf/*.c)
HEADERS = $(wildcard include/*.h)
OBJECTS = $(SOURCES:%.c=%.o)

default: $(TARGET)
all: default

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	ar rs $@ $(OBJECTS)

clean:
	-rm -f $(OBJECTS)

fclean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)

re: fclean all

.PHONY: default all clean fclean re