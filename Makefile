NAME = ./minishell

LIBFT_DIR   := ./submodules/42_libft
GNL_DIR     := ./submodules/42_get_next_line

SRC =	./src/0-expander/env_expand_util.c ./src/0-expander/env_expansion.c \
		./src/0-expander/expand_util.c ./src/0-expander/expander.c \
		./src/0-expander/should_expand.c \
		./src/0-expander/wildcard.c \
		./src/0-expander/wildcard_util.c \
		./src/1-lexer/count_tokens.c \
		./src/1-lexer/detect_token_type.c \
		./src/1-lexer/lexer.c \
		./src/1-lexer/lexer_util.c \
		./src/1-lexer/ms_split.c \
		./src/1-lexer/put_space_between_tokens.c \
		./src/2-validator/check_files.c \
		./src/2-validator/check_tokens.c \
		./src/2-validator/validator.c \
		./src/3-parser/parser.c \
		./src/3-parser/parser_defaults.c \
		./src/3-parser/parser_util.c \
		./src/3-parser/substring_util.c \
		./src/3-parser/switch_redir_args.c \
		./src/4-executor/builtins/cd.c \
		./src/4-executor/builtins/echo.c \
		./src/4-executor/builtins/env.c \
		./src/4-executor/builtins/exit.c \
		./src/4-executor/builtins/export.c \
		./src/4-executor/builtins/pwd.c \
		./src/4-executor/builtins/unset.c \
		./src/4-executor/builtins/util/envcp_utils.c \
		./src/4-executor/execute_command_utils.c \
		./src/4-executor/executer.c \
		./src/4-executor/node_executer.c \
		./src/4-executor/operators/heredoc.c \
		./src/4-executor/operators/logical.c \
		./src/4-executor/operators/pipe.c \
		./src/4-executor/operators/redirect.c \
		./src/4-executor/operators/util.c \
		./src/4-executor/pid_list.c \
		./src/4-executor/util.c \
		./src/main.c \
		./src/program_loop/execute_input.c \
		./src/program_loop/garbage_collector/garbage_collector.c \
		./src/program_loop/garbage_collector/gc_util.c \
		./src/program_loop/handle_input.c \
		./src/program_loop/input_loop.c \
		./src/program_loop/logo.c \
		./src/program_loop/signals.c \
		./src/util/error.c \
		./src/util/ft_strjoinfree.c \
		./src/util/printing.c
OBJ = $(SRC:.c=.o)

SRC_DIR = ./src

CFLAGS		:=	-Wall -Werror -Wextra -g
HEADER		:=	-I./include/
LIBS		:=	-L$(LIBFT_DIR) -lft \
				-L$(GNL_DIR) -lftgnl \
				-lreadline

RED=$(shell tput setaf 1)
GREEN=$(shell tput setaf 2)
YELLOW=$(shell tput setaf 3)
BLUE=$(shell tput setaf 4)
PURPLE=$(shell tput setaf 5)
CYAN=$(shell tput setaf 6)
NC=$(shell tput sgr0) # Reset

all: $(NAME)
	@echo "$(GREEN)$(NAME) is done!$(NC)"

$(NAME): $(LIBFT_DIR)/libft.a $(GNL_DIR)/libftgnl.a $(OBJ)
	@echo "$(CYAN)Linking $(NAME)...$(NC)"
	@cc $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)
%.o: %.c
	@mkdir -p $(@D)
	@printf "$(PURPLE)$(notdir $<) $(NC)"
	@cc -c $< $(CFLAGS) $(HEADER) -o $@

$(LIBFT_DIR)/libft.a:
	@if [ ! -d "submodules/42_libft" ]; then git submodule update --init --remote; fi
	@echo "$(YELLOW)Making libft...$(NC)"
	@make -C $(LIBFT_DIR) > /dev/null
$(GNL_DIR)/libftgnl.a:
	@if [ ! -d "submodules/42_get_next_line" ]; then git submodule update --init --remote; fi
	@echo "$(YELLOW)Making get_next_line...$(NC)"
	@make -C $(GNL_DIR) > /dev/null


clean:
	@echo "$(RED)\nCleaning up object files...$(NC)"
	@rm -f $(OBJ) $(BOBJ) > /dev/null
	@make -C $(LIBFT_DIR) clean > /dev/null
	@make -C $(GNL_DIR) clean > /dev/null
	@echo "$(RED)"
	@echo "$(NC)"
fclean: clean
	@echo "$(RED)Removing binaries...$(NC)"
	@make -C $(LIBFT_DIR) fclean > /dev/null
	@make -C $(GNL_DIR) fclean > /dev/null
	@rm -f $(NAME) $(BNAME)
re: fclean all

.PHONY: all clean fclean re comp normcomp