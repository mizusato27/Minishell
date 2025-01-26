# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/16 19:59:21 by ynihei            #+#    #+#              #
#    Updated: 2025/01/26 15:30:33 by ynihei           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS_DIR	= srcs
SRCS		= $(addprefix $(SRCS_DIR)/, main.c)

OBJS		= ${SRCS:.c=.o}

NAME		= minishell

CC			= cc
CFLAGS			= -Wall -Wextra -Werror
LDFLAGS     = -lreadline

LIBFT       = libft/libft.a

all:		${NAME}

.c.o:
			${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${LIBFT}:
			make -C ./libft

${NAME}:	${OBJS} ${LIBFT}
			${CC} ${CFLAGS} ${OBJS} ${LIBFT} ${LDFLAGS} -o ${NAME}

clean:
			make clean -C ./libft
			${RM} ${OBJS} 

fclean:		clean
			make fclean -C ./libft
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re bonus
