# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: tblaudez <tblaudez@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2021/03/31 15:06:14 by tblaudez      #+#    #+#                  #
#    Updated: 2021/05/12 10:06:26 by tblaudez      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

TARGET := ft_nm
CC := clang
CFLAGS := -Wall -Wextra -Werror -I include/ -I libft/include/ -std=c99 $(EXTRA_FLAGS)

SOURCES := $(shell find src/ -name "*.c")
HEADERS := $(shell find include/ -name "*.h")
OBJECTS := $(SOURCES:%.c=%.o)

LIBFT := libft/libft.a
LIBFT_FLAGS := -Llibft/ -lft

all: $(TARGET)

$(TARGET): $(LIBFT) $(OBJECTS)
	@printf "✅ \e[96;1m%s\e[0m\n" $(TARGET)
	@$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT_FLAGS) -o $@

$(LIBFT):
	@$(MAKE) -C libft/

%.o: %.c $(HEADERS)
	@printf "🔄 \e[34m%s: \e[32m%s\e[0m\n" $(TARGET) $@
	@$(CC) $(CFLAGS) -c $< -o $@


clean:
	@$(MAKE) -C libft/ clean
	@printf "🗑  \e[31m%s\e[0m\n" $(OBJECTS)
	@rm -f $(OBJECTS)

fclean: clean
	@$(MAKE) -C libft/ fclean
	@printf "🗑  \e[31;1m%s\e[0m\n" $(TARGET)
	@rm -f $(TARGET)

re: fclean all

.PHONY: default all clean fclean re

.SILENT:
