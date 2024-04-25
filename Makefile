NAME = ./minishell

LIBFT_DIR   := ./submodules/42_libft
GNL_DIR     := ./submodules/42_get_next_line

SRC = $(shell find ./src -name "*.c")
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

SRC_DIR = ./src
OBJ_DIR = ./obj

CFLAGS		:=	-Wall -Werror -Wextra -g #-fsanitize=address
HEADER		:=	-I./include/
LIBS		:=	-L$(LIBFT_DIR) -lft \
				-L$(GNL_DIR) -lftgnl \
				-lreadline

$(NAME): pre_compile $(LIBFT_DIR)/libft.a $(GNL_DIR)/libftgnl.a $(OBJ) pre_link
	@echo "$(CYAN)Linking $(NAME)...$(NC)"
	@cc $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@printf "$(PURPLE)$(notdir $<) $(NC)"
	@cc -c $< $(CFLAGS) $(HEADER) -o $@

$(LIBFT_DIR)/libft.a:
	@echo "$(YELLOW)Making libft...$(NC)"
	@make -C $(LIBFT_DIR) comp > /dev/null
$(GNL_DIR)/libftgnl.a:
	@echo "$(YELLOW)Making get_next_line...$(NC)"
	@make -C $(GNL_DIR) > /dev/null


RED=$(shell tput setaf 1)
GREEN=$(shell tput setaf 2)
YELLOW=$(shell tput setaf 3)
BLUE=$(shell tput setaf 4)
PURPLE=$(shell tput setaf 5)
CYAN=$(shell tput setaf 6)
NC=$(shell tput sgr0) # Reset

pre_compile:
	@echo "$(PURPLE)\nStarting compilation...$(NC)"
pre_link:
	@echo "$(CYAN)\n\nStarting linking...$(NC)"

all: $(NAME)
	@echo "$(GREEN)$(NAME) is done!$(NC)"
clean:
	@echo "$(RED)\nCleaning up object files...$(NC)"
	@rm -f $(OBJ) $(BOBJ) > /dev/null
	@make -C $(LIBFT_DIR) clean > /dev/null
	@make -C $(GNL_DIR) clean > /dev/null
	@echo "$(RED)"
	rm -rf $(OBJ_DIR)
	@echo "$(NC)"
fclean: clean
	@echo "$(RED)Removing binaries...$(NC)"
	@make -C $(LIBFT_DIR) fclean > /dev/null
	@make -C $(GNL_DIR) fclean > /dev/null
	@rm -f $(NAME) $(BNAME)
re: fclean all

comp:
	@echo "$(GREEN)Updating submodules and recompiling...$(NC)"
	@git submodule update --init --remote
	@make re
	@make clean
	@clear
normcomp:
	@echo "$(GREEN)Updating submodules...$(NC)"
	@git submodule update --init --remote
	@echo "$(GREEN)\nChecking norminette...$(NC)"
	@cd ./include && norminette
	@cd ./src && norminette
	@make re
	@make clean
	@clear

.PHONY: all clean fclean re comp normcomp
