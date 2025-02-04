/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:30:52 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/04 16:30:51 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdbool.h>

#ifndef PATH_MAX
# define PATH_MAX 10000
#endif

# define CHILD_PROCESS 0

//error.c
void    malloc_error();
void	err_exit(const char *location, const char *msg, int status);
void	error(char *msg);

//exec.c
int interpret(char *line);

#endif