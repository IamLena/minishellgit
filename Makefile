NAME			=	minishell
FLAGS			=	-Wall -Werror -Wextra -g

LIBFT			=	libft.a
LIBFT_DIR		=	libft
FTPRINTF		=	libftprintf.a
FTPRINTF_DIR	=	ft_printf
HEADERS			=	srcs/minishell.h

SRC				=	srcs/main.c		srcs/cmdlist.c		srcs/parser_token_to_cmd.c		srcs/execcmdloop.c\
					srcs/parser_tokens.c				srcs/findexeccmd.c \
					srcs/tools1_arrays.c	srcs/tools2.c				srcs/tools3_str.c \
					srcs/tools4_output.c	srcs/tools5_free.c			srcs/tools6_redirects.c \
					srcs/exec_cd.c			srcs/exec_echo.c			srcs/exec_env.c		srcs/exec_export.c \
					srcs/exec_exit.c		srcs/exec_unset.c			srcs/exec_pwd.c		\
					srcs/args.c				srcs/execve.c				srcs/empty_vars.c

OBJ = $(SRC:.c=.o)

.PHONY: all clean fclean re run

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT) $(FTPRINTF)
	echo "out make"
	gcc $(FLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(FTPRINTF) -L. -lft

%.o:%.c $(HEADERS)
	gcc -c -g $< -o $@

$(LIBFT):
	$(MAKE) bonus -C $(LIBFT_DIR)
	cp $(LIBFT_DIR)/$(LIBFT) ./$(LIBFT)

$(FTPRINTF):
	$(MAKE) -C $(FTPRINTF_DIR)
	cp $(FTPRINTF_DIR)/$(FTPRINTF) ./$(FTPRINTF)

clean:
	$(MAKE) clean -C $(LIBFT_DIR)
	$(MAKE) clean -C $(FTPRINTF_DIR)
	rm -rf $(OBJ)

fclean:
	$(MAKE) fclean -C $(LIBFT_DIR)
	$(MAKE) fclean -C $(FTPRINTF_DIR)
	rm -rf $(OBJ) $(NAME) $(LIBFT) $(FTPRINTF)

re: fclean all
