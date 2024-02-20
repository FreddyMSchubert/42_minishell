NAME = ./crash

LIBFT_DIR   := ./submodules/42_libft
FTPRINTF_DIR:= ./submodules/42_ft_printf
FTGNL_DIR   := ./submodules/42_get_next_line

SRC = $(shell find ./src -name "*.c")
OBJ = $(SRC:.c=.o)

CFLAGS		:=	-Wall -Werror -Wextra \
				-Wunreachable-code -Ofast -g -fsanitize=address
HEADER		:=	-I./include/
LIBS		:=	-L$(LIBFT_DIR) -lft \
				-L$(FTPRINTF_DIR) -lftprintf \
				-L$(FTGNL_DIR) -lftgnl

$(NAME): $(LIBFT_DIR)/libft.a $(FTPRINTF_DIR)/ftprintf.a $(FTGNL_DIR)/ftgnl.a $(OBJ)
	@cc $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)
%.o: %.c
	@cc -c $< $(CFLAGS) $(HEADER) -o $@

$(LIBFT_DIR)/libft.a:
	make -C $(LIBFT_DIR) comp
$(FTPRINTF_DIR)/ftprintf.a:
	make -C $(FTPRINTF_DIR)
$(FTGNL_DIR)/ftgnl.a:
	make -C $(FTGNL_DIR)

all: $(NAME)
clean:
	@rm -f $(OBJ) $(BOBJ)
	make -C $(LIBFT_DIR) clean
	make -C $(FTPRINTF_DIR) clean
	make -C $(FTGNL_DIR) clean
fclean: clean
	make -C $(LIBFT_DIR) fclean
	make -C $(FTPRINTF_DIR) fclean
	make -C $(FTGNL_DIR) fclean
	@rm -f $(NAME) $(BNAME)
re: fclean all

comp:
	@git submodule update --init --remote
	@make re
	@make clean
	@clear
normcomp:
	@git submodule update --init --remote
	@cd ./include && norminette
	@cd ./src && norminette
	@make re
	@make clean
	@clear

.PHONY: all clean fclean re comp normcomp