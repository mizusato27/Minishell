# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/16 19:59:21 by ynihei            #+#    #+#              #
#    Updated: 2025/03/08 02:23:23 by ynihei           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS_DIR		= srcs
BUILTIN_DIR		= $(SRCS_DIR)/builtin
ENV_DIR			= $(SRCS_DIR)/env
EXPAND_DIR		= $(SRCS_DIR)/expand
PARSE_DIR		= $(SRCS_DIR)/parse
PIPE_DIR		= $(SRCS_DIR)/pipe
REDIRECT_DIR	= $(SRCS_DIR)/redirect
TOKENIZE_DIR	= $(SRCS_DIR)/tokenize
UTILS_DIR		= $(SRCS_DIR)/utils
FINISH_DIR		= $(SRCS_DIR)/finish
SIGNAL_DIR		= $(SRCS_DIR)/signal
EXEC_DIR		= $(SRCS_DIR)/exec

SRCS		= $(addprefix $(SRCS_DIR)/, main.c) \
			  $(addprefix $(BUILTIN_DIR)/, builtin.c exit.c export.c unset.c env.c cd.c echo.c pwd.c) \
			  $(addprefix $(ENV_DIR)/, env.c map_set_from_string.c map_helper.c map_set_value.c map_get_value.c) \
			  $(addprefix $(EXPAND_DIR)/, expand.c quote_removal.c special_param.c variable.c) \
			  $(addprefix $(PARSE_DIR)/, parse_redir.c parse_utils.c parse.c) \
			  $(addprefix $(PIPE_DIR)/, pipe.c) \
			  $(addprefix $(REDIRECT_DIR)/, here_document.c open_file.c redirect.c stash_fd.c) \
			  $(addprefix $(TOKENIZE_DIR)/, token.c tokenize.c tokenize_helper.c) \
			  $(addprefix $(UTILS_DIR)/, ft_close.c ft_dup2.c ft_pipe.c utils.c) \
			  $(addprefix $(FINISH_DIR)/, destructor.c error.c error2.c) \
			  $(addprefix $(SIGNAL_DIR)/, signal.c reset_signal.c) \
			  $(addprefix $(EXEC_DIR)/, exec.c exec_no_builtin.c token_to_arg.c)

OBJS_DIR 	= objs
OBJS		= ${SRCS:%.c=$(OBJS_DIR)/%.o}

NAME		= minishell

CC			= cc
# CFLAGS		= -Wall -Wextra -Werror -I includers/ \
# 				#  -fsanitize=address
# LDFLAGS     = -lreadline
## macOS用
RLDIR		= $(shell brew --prefix readline)
CFLAGS		= -Wall -Wextra -Werror -I includers/ -I$(RLDIR)/include
LDFLAGS		= -L$(RLDIR)/lib -lreadline

LIBFT		= libft/libft.a

all:		${NAME}

${LIBFT}:
			make -C ./libft

# ${NAME}:	${OBJS} ${LIBFT}
# 			${CC} ${CFLAGS} ${OBJS} ${LDFLAGS} ${LIBFT} -o ${NAME}
${NAME}:	${OBJS} ${LIBFT}
			${CC} ${CFLAGS} ${OBJS} ${LIBFT} ${LDFLAGS} -o ${NAME}

# 依存するソースファイルを個別にオブジェクトファイルへコンパイル
$(OBJS_DIR)/%.o:	%.c
			mkdir -p $(dir $@)
			${CC} ${CFLAGS} -c $< -o $@

clean:
			make clean -C ./libft
			${RM} -r ${OBJS_DIR}

fclean:		clean
			make fclean -C ./libft
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
