NAME := libft.a

CC := cc
CFLAGS := -Wall -Wextra -Werror -g3

SRC_DIR := src/
SRC := $(wildcard $(SRC_DIR)**/*.c)

OBJ_DIR := .obj/
OBJ := $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRC))
###############################################################################

all: $(NAME)

test: $(NAME) $(TEST_OBJ) $(TEST_BIN)

$(NAME): $(OBJ)
	ar rcs $@ $?

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@ mkdir -p $(OBJ_DIR)
	@ mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

print-%  : ; @echo $* = $($*)

.PHONY: all clean fclean re
