# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/16 19:59:21 by ynihei            #+#    #+#              #
#    Updated: 2025/01/26 16:31:50 by ynihei           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS_DIR	= srcs
SRCS		= $(addprefix $(SRCS_DIR)/, main.c)

OBJS_DIR 	= objs
OBJS		= ${SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o}

NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -I includers/
LDFLAGS     = -lreadline

LIBFT       = libft/libft.a

all:		${NAME}

${LIBFT}:
			make -C ./libft

${NAME}:	${OBJS} ${LIBFT}
			${CC} ${CFLAGS} ${OBJS} ${LIBFT} ${LDFLAGS} -o ${NAME}

$(OBJS_DIR)/%.o:	$(SRCS_DIR)/%.c
			${CC} ${CFLAGS} -c $< -o $@

clean:
			make clean -C ./libft
			${RM} ${OBJS} 

fclean:		clean
			make fclean -C ./libft
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
