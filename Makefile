CC := cc -g
CFLAGS := -Wall -Wextra -Werror
SYSLIB := -lreadline
LIBFT := libft/libft.a

NAME := minishell

SRC_DIR := src/
SRC := src/bltin/blt_env.c src/bltin/blt_rest.c src/exec/builtins_utils.c src/exec/exec.c src/exec/exec_cmd.c src/exec/exec_grp.c src/exec/exec_pipe_utils.c src/exec/exec_redir_utils.c src/exec/exec_utils_1.c src/exec/exec_utils_2.c src/exec/exec_utils_3.c src/exec/exec_utils_4.c src/exec/exec_utils_5.c src/exec/expand.c src/exec/expand_utils_2.c src/exec/expand_utils.c src/exec/fd_utils.c src/exec/redir_star_handler.c src/parse/ast_create.c src/parse/ast_destroy.c src/parse/ast_print.c src/parse/parse1.c src/parse/parse2.c src/readline/ms_readline.c src/readline/ms_signal.c src/readline/redir.c src/utils/exit_utils.c src/utils/parse_utils.c src/utils/spl_utils.c src/utils/string_utils.c
SRC += src/main.c

OBJ_DIR := .obj/
OBJ := $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRC))
OBJ += .obj/main.o

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $^ $(LIBFT) -o $@ $(SYSLIB)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@ mkdir -p $(OBJ_DIR)
	@ mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(LIBFT):
	make -C libft/

clean:
	make -C libft/ clean
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

print-%  : ; @echo $* = $($*)
:PHONY all clean fclean re
