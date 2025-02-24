NAME := libft.a

CC := cc
CFLAGS := -Wall -Wextra -Werror

SRC_DIR := src/
SRC :=src/ft_ctype/ft_isalnum.c src/ft_ctype/ft_isalpha.c src/ft_ctype/ft_isascii.c src/ft_ctype/ft_isdigit.c src/ft_ctype/ft_isprint.c src/ft_ctype/ft_tolower.c src/ft_ctype/ft_toupper.c src/ft_ints/ft_ibsearch.c src/ft_ints/ft_imath.c src/ft_ints/ft_isort.c src/ft_ints/ft_iswap.c src/ft_lst/ft_lstadd_back.c src/ft_lst/ft_lstadd_front.c src/ft_lst/ft_lstclear.c src/ft_lst/ft_lstdelone.c src/ft_lst/ft_lstiter.c src/ft_lst/ft_lstlast.c src/ft_lst/ft_lstmap.c src/ft_lst/ft_lstnew.c src/ft_lst/ft_lstsize.c src/ft_printf/ft_printf.c src/ft_printf/ft_printf_utils.c src/ft_printf/int_printers.c src/ft_printf/itoa_base.c src/ft_printf/parser.c src/ft_printf/printer_utils1.c src/ft_printf/printer_utils2.c src/ft_printf/ptr_printers.c src/ft_printf/tokenizer.c src/ft_stdio/ft_putchar_fd.c src/ft_stdio/ft_putendl_fd.c src/ft_stdio/ft_putnbr_fd.c src/ft_stdio/ft_putstr_fd.c src/ft_stdio/get_next_line.c src/ft_stdlib/ft_atoi.c src/ft_stdlib/ft_calloc.c src/ft_stdlib/ft_free_split.c src/ft_stdlib/ft_itoa.c src/ft_stdlib/ft_strtoll.c src/ft_string/ft_bzero.c src/ft_string/ft_memchr.c src/ft_string/ft_memcmp.c src/ft_string/ft_memcpy.c src/ft_string/ft_memmove.c src/ft_string/ft_memset.c src/ft_string/ft_split.c src/ft_string/ft_strcat.c src/ft_string/ft_strchr.c src/ft_string/ft_strcmp.c src/ft_string/ft_strcpy.c src/ft_string/ft_strdup.c src/ft_string/ft_striteri.c src/ft_string/ft_strjoin.c src/ft_string/ft_strlcat.c src/ft_string/ft_strlcpy.c src/ft_string/ft_strlen.c src/ft_string/ft_strmapi.c src/ft_string/ft_strncat.c src/ft_string/ft_strncmp.c src/ft_string/ft_strnstr.c src/ft_string/ft_strrchr.c src/ft_string/ft_strtrim.c src/ft_string/ft_substr.c

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
