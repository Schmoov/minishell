CC := cc #-g
CFLAGS := -Wall -Wextra #-Werror
SYSLIB := -lreadline
LIBFT := libft/libft.a

NAME := minishell

SRC_DIR := src/
SRC := $(wildcard $(SRC_DIR)*.c)

OBJ_DIR := .obj/
OBJ := $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRC))

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(SYSLIB) $^ $(LIBFT) -o $@ 

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@ mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(LIBFT):
	make -C libft/

clean:
	make -C libft/ fclean
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

:PHONY all clean fclean re
