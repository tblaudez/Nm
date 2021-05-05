# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: tblaudez <tblaudez@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2021/03/31 15:06:14 by tblaudez      #+#    #+#                  #
#    Updated: 2021/05/05 08:46:22 by tblaudez      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

TARGET := ft_nm
CC := clang
CFLAGS := -Wall -Wextra -Werror -I include/ -I libft/include/ $(EXTRA_FLAGS)
LIBRARIES := -Llibft/ -lft

SOURCES := $(shell find src/ -name "*.c")
HEADERS := $(shell find include/ -name "*.h")
OBJECTS := $(SOURCES:%.c=%.o)

LIBFT := libft/libft.a

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
