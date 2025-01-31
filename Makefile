CC := cc #-g
CFLAGS := -Wall -Wextra #-Werror
SYSLIB := -lreadline
LIBFT := libft/libft.a

NAME := minishell

SRC_DIR := src/
SRC := $(wildcard $(SRC_DIR)**/*.c)
SRC += src/main.c

OBJ_DIR := .obj/
OBJ := $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRC))
OBJ += .obj/main.o

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(SYSLIB) $^ $(LIBFT) -o $@ 

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
