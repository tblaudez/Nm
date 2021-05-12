# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: tblaudez <tblaudez@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2021/03/31 15:28:54 by tblaudez      #+#    #+#                  #
#    Updated: 2021/05/12 10:06:21 by tblaudez      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

TARGET := libft.a
CC := clang
CFLAGS := -Wall -Wextra -Werror -I include/ -std=c99 $(EXTRA_FLAGS)

SOURCES := $(shell find src/ -name "*.c")
HEADERS := $(shell find include/ -name "*.h")
OBJECTS := $(SOURCES:%.c=%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@printf "✅ \e[96;1m%s\e[0m\n" $(TARGET)
	@ar rs $@ $(OBJECTS) 2>&-

%.o: %.c $(HEADERS)
	@printf "🔄 \e[34m%s: \e[32m%s\e[0m\n" $(TARGET) $@
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@printf "🗑  \e[31m%s\e[0m\n" $(OBJECTS)
	@rm -f $(OBJECTS)

fclean: clean
	@printf "🗑  \e[31;1m%s\e[0m\n" $(TARGET)
	@rm -f $(TARGET)

re: fclean all

.PHONY: default all clean fclean re