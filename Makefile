# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/16 19:59:21 by ynihei            #+#    #+#              #
#    Updated: 2025/02/23 17:18:35 by ynihei           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS_DIR	= srcs
BUILTIN_DIR	= $(SRCS_DIR)/builtin
ENV_DIR		= $(SRCS_DIR)/env

SRCS		= $(addprefix $(SRCS_DIR)/, main.c error.c exec.c expand.c tokenize.c tokenize_helper.c \
				destructor.c parse.c utils.c signal.c redirect.c pipe.c) \
			  $(addprefix $(BUILTIN_DIR)/, builtin.c exit.c) \
			  $(addprefix $(ENV_DIR)/, env.c map.c map_helper.c map_create.c )

OBJS_DIR 	= objs
OBJS		= ${SRCS:%.c=$(OBJS_DIR)/%.o}

NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -I includers/
LDFLAGS     = -lreadline

LIBFT       = libft/libft.a

all:		${NAME}

${LIBFT}:
			make -C ./libft

${NAME}:	${OBJS} ${LIBFT}
			${CC} ${CFLAGS} ${OBJS} ${LDFLAGS} ${LIBFT} -o ${NAME}

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
