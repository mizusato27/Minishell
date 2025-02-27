# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/16 19:59:21 by ynihei            #+#    #+#              #
#    Updated: 2025/02/27 16:40:02 by mizusato         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS_DIR		= srcs
BUILTIN_DIR		= $(SRCS_DIR)/builtin
ENV_DIR			= $(SRCS_DIR)/env
EXPAND_DIR		= $(SRCS_DIR)/expand
REDIRECT_DIR	= $(SRCS_DIR)/redirect

SRCS		= $(addprefix $(SRCS_DIR)/, main.c error.c exec.c tokenize.c tokenize_helper.c \
				destructor.c parse.c utils.c signal.c pipe.c) \
			  $(addprefix $(BUILTIN_DIR)/, builtin.c exit.c export.c unset.c env.c cd.c echo.c pwd.c) \
			  $(addprefix $(ENV_DIR)/, env.c map.c map_helper.c map_create.c) \
			  $(addprefix $(EXPAND_DIR)/, expand.c quote_removal.c special_param.c variable.c) \
			  $(addprefix $(REDIRECT_DIR)/, here_document.c open_file.c redirect.c stash_fd.c)

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
