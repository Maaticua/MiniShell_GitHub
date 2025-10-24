# Standard
NAME				= minishell

# Directories
LIBFT				= ./libft/libft.a
INC					= inc/
SRC_DIR				= src/
OBJ_DIR				= obj/

# Compiler and CFlags
CC					=	cc
CFLAGS				=	-Wall -Werror -Wextra -g -I
LDFLAGS				=	-lreadline
RM					=	rm -f

# Concatenate all source files


SRCS 				=	$(SRC_DIR)main_loop.c						\
						$(SRC_DIR)main_loop_handlers.c				\
						$(SRC_DIR)minishell.c						\
						$(SRC_DIR)prompt.c							\
						$(SRC_DIR)builtins/cd.c						\
						$(SRC_DIR)builtins/echo.c					\
						$(SRC_DIR)builtins/env.c					\
						$(SRC_DIR)builtins/exit.c					\
						$(SRC_DIR)builtins/export.c					\
						$(SRC_DIR)builtins/export_utils.c			\
						$(SRC_DIR)builtins/pwd.c					\
						$(SRC_DIR)builtins/unset.c					\
						$(SRC_DIR)executor/builtin.c				\
						$(SRC_DIR)executor/executor.c				\
						$(SRC_DIR)executor/executor_name.c			\
						$(SRC_DIR)executor/executor_redir.c			\
						$(SRC_DIR)executor/executor_run.c			\
						$(SRC_DIR)executor/executor_heredoc.c		\
						$(SRC_DIR)executor/executor_utils.c			\
						$(SRC_DIR)executor/pipes_utils.c			\
						$(SRC_DIR)executor/pipes.c					\
						$(SRC_DIR)executor/redirections.c			\
						$(SRC_DIR)executor/signals.c				\
						$(SRC_DIR)executor/signals2.c				\
						$(SRC_DIR)parser/expand.c					\
						$(SRC_DIR)parser/expand_utils.c				\
						$(SRC_DIR)parser/expand_is_n_append.c		\
						$(SRC_DIR)parser/expand_var_utils.c			\
						$(SRC_DIR)parser/lexer.c					\
						$(SRC_DIR)parser/parser.c					\
						$(SRC_DIR)parser/parser_pipe.c				\
						$(SRC_DIR)parser/parser_token_steps.c		\
						$(SRC_DIR)parser/quotes.c					\
						$(SRC_DIR)parser/tokenizer.c				\
						$(SRC_DIR)parser/tokenizer_quote_spc_utils.c\
						$(SRC_DIR)parser/tokenizer_loop_utils.c		\
						$(SRC_DIR)utils/garbage_collector.c			\
						$(SRC_DIR)utils/garbage_collector_utils.c	\
						$(SRC_DIR)utils/garbage_collector_utils2.c	\
						$(SRC_DIR)utils/env.c						\
						$(SRC_DIR)utils/env2.c						\
						$(SRC_DIR)utils/env_utils.c					\
						$(SRC_DIR)utils/error.c						\
						$(SRC_DIR)utils/str_utils.c					\
						$(SRC_DIR)utils/utils.c						\
						$(SRC_DIR)utils/shell.c						\


# Apply the pattern substitution to each source file in SRC and produce a corresponding list of object files in the OBJ_DIR
OBJ 				= $(patsubst %.c,$(OBJ_DIR)/%.o, $(SRCS))


# Build rules
start:
					@make all

$(LIBFT):
					@make -C ./libft

all: 				$(NAME)

$(NAME): 			$(OBJ) $(LIBFT)
					@$(CC) $(CFLAGS) $(INC) $(OBJ) $(LIBFT) -o $(NAME) $(LDFLAGS)

# Compile object files from source files
$(OBJ_DIR)/%.o: %.c
					@mkdir -p $(@D)
					@$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
					@$(RM) -r $(OBJ_DIR)
					@make clean -C ./libft

fclean: 			clean
					@$(RM) $(NAME)
					@$(RM) $(LIBFT)

re: 				fclean all

# Phony targets represent actions not files
.PHONY: 			start all clean fclean re
