# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/09 20:09:38 by bluzi             #+#    #+#              #
#    Updated: 2023/08/03 15:48:28 by yorlians         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SILENT:

NAME	:= minishell

CFLAGS	:=	-Wextra -Wall -Werror

HEADERS	:=	-I include -I/Users/$(USER)/.brew/opt/readline/include
LIBS	:=	-L/Users/$(USER)/.brew/opt/readline/lib -lreadline

BLUE		=	\033[0;94m
GREEN		=	\033[0;32m
RESET		=	\033[0m

SRCS	:=	src/main.c									\
			src/init.c									\
			src/garbage_collector/garbage_collector.c	\
			src/temp.c									\
			src/signals.c								\
			src/builtins/builtin.c						\
			src/builtins/echo.c							\
			src/builtins/cd.c							\
			src/builtins/pwd.c							\
			src/builtins/env.c							\
			src/builtins/export.c						\
			src/builtins/export_syntax.c				\
			src/builtins/print_errors.c					\
			src/builtins/print_errors_2.c				\
			src/builtins/unset.c						\
			src/builtins/exit.c							\
			src/utils/char.c							\
			src/utils/string.c							\
			src/utils/string_duplicate.c				\
			src/utils/utils.c							\
			src/utils/itoa.c							\
			src/utils/free.c							\
			src/utils/split.c							\
			src/linked_list/token_list.c				\
			src/linked_list/execution_list.c			\
			src/parser/parser.c							\
			src/parser/tokenizer.c						\
			src/parser/heredoc.c						\
			src/parser/expansions.c						\
			src/parser/word_split.c						\
			src/parser/quote.c							\
			src/execution/execution.c					\
			src/execution/command.c						\
			src/execution/redirections.c				\
			src/get_next_line/get_next_line.c			\
			src/get_next_line/get_next_line_utils.c		\

OBJS	:= ${SRCS:.c=.o}

all: $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

$(NAME): $(OBJS)
	@$(CC) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)
	echo "$(GREEN)✅🚀 Program compiled successfully 🚀✅$(RESET)"

clean:
	@rm -f $(OBJS)
	echo "$(BLUE)✨🧹🧺✨ Removed object files ✨🧺🧹✨$(RESET)"

fclean: clean
	@rm -f $(NAME)
	echo "$(BLUE)✨💦🛁✨ Removed executable ✨🛁💦✨$(RESET)"

re: clean all

.PHONY: all, clean, fclean, re