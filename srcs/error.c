/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:23:43 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/08 19:48:26 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	syntax_error = false;

void	err_exit(const char *location, const char *msg, int status)
{
    write(2, "minishell: ", 11);
    write(2, location, strlen(location));
    write(2, ": ", 2);
    write(2, msg, ft_strlen(msg));
    write(2, "\n", 1);
    exit(status);
}

//構文エラーがある場合に単語の最後までスキップ
void	tokenize_error(const char *location, int *j, char *line)
{
	syntax_error = true;
	dprintf(STDERR_FILENO, "minishell: syntax error near %s\n", location);
	while (line[*j])
		(*j)++;
}

void	error(char *msg)
{
	printf("%s\n", msg);
	exit(2);
}



