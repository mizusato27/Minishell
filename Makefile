# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/16 19:59:21 by ynihei            #+#    #+#              #
#    Updated: 2025/01/26 15:50:29 by ynihei           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS_DIR	= srcs
SRCS		= $(addprefix $(SRCS_DIR)/, main.c)

OBJS_DIR 	= objs
OBJS		= ${SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o}

NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
LDFLAGS     = -lreadline

# LIBFT       = libft/libft.a

all:		${NAME}

# ${LIBFT}:
# 			make -C ./libft

${NAME}:	${OBJS} 
			${CC} ${CFLAGS} ${OBJS} ${LDFLAGS} -o ${NAME}

$(OBJS_DIR)/%.o:	$(SRCS_DIR)/%.c
			${CC} ${CFLAGS} -c $< -o $@

clean:
			${RM} ${OBJS} 

fclean:		clean
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
