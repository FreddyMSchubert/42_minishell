NAME = ./crash

LIBFT_DIR   := ./submodules/42_libft
FTPRINTF_DIR:= ./submodules/42_ft_printf

SRC = $(shell find ./src -name "*.c")
OBJ = $(SRC:.c=.o)

CFLAGS		:=	-Wall -Werror -Wextra -g -fsanitize=address
HEADER		:=	-I./include/
LIBS		:=	-L$(LIBFT_DIR) -lft \
				-L$(FTPRINTF_DIR) -lftprintf \
				-L$(FTGNL_DIR) -lftgnl \
				-lreadline

$(NAME): pre_compile $(LIBFT_DIR)/libft.a $(FTPRINTF_DIR)/ftprintf.a $(FTGNL_DIR)/ftgnl.a $(OBJ) pre_link
	@echo "$(CYAN)Linking $(NAME)...$(NC)"
	@cc $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME) > /dev/null
%.o: %.c
	@printf "$(PURPLE) $(notdir $<)$(NC)"
	@cc -c $< $(CFLAGS) $(HEADER) -o $@ > /dev/null

$(LIBFT_DIR)/libft.a:
	@echo "$(YELLOW)Making libft...$(NC)"
	@make -C $(LIBFT_DIR) comp > /dev/null
$(FTPRINTF_DIR)/ftprintf.a:
	@echo "$(YELLOW)Making ft_printf...$(NC)"
	@make -C $(FTPRINTF_DIR) > /dev/null

RED=\033[0;31m		# removing stuff
GREEN=\033[0;32m	# comp / normcomp / general messages
YELLOW=\033[0;33m	# submodules
BLUE=\033[0;34m
PURPLE=\033[0;35m	# compiling
CYAN=\033[0;36m		# linking
NC=\033[0m # Reset

pre_link:
	@echo "$(CYAN)Starting linking...$(NC)"
pre_compile:
	@echo "$(PURPLE)Starting compilation...$(NC)"

all: $(NAME)
	@echo "$(GREEN)$(NAME) is done!$(NC)"
clean:
	@echo "$(RED)Cleaning up object files...$(NC)"
	@rm -f $(OBJ) $(BOBJ) > /dev/null
	@make -C $(LIBFT_DIR) clean > /dev/null
	@make -C $(FTPRINTF_DIR) clean > /dev/null
	@make -C $(FTGNL_DIR) clean > /dev/null
fclean: clean
	@echo "$(RED)Removing binaries...\n$(NC)"
	@make -C $(LIBFT_DIR) fclean > /dev/null
	@make -C $(FTPRINTF_DIR) fclean > /dev/null
	@make -C $(FTGNL_DIR) fclean > /dev/null
	@rm -f $(NAME) $(BNAME)
re: fclean all

comp:
	@echo "$(GREEN)Updating submodules and recompiling...$(NC)"
	@git submodule update --init --remote
	@make re
	@make clean
	@clear
	@./crash
normcomp:
	@echo "$(GREEN)Checking Norminette, updating submodules & recompiling...$(NC)"
	@git submodule update --init --remote
	@cd ./include && norminette
	@cd ./src && norminette
	@make re
	@make clean
	@clear

.PHONY: all clean fclean re comp normcomp