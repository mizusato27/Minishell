/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_document.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 22:43:20 by mizusato          #+#    #+#             */
/*   Updated: 2025/02/28 22:43:22 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	readline_interrupted = false;

int	read_here_document(const char *delimiter)
{
	char *line;
	int pipe_fd[2];

	if (pipe(pipe_fd) < 0)
		fatal_error("pipe");
	readline_interrupted = false;
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			break ;
		if (readline_interrupted)
		{
			free(line);
			break ;
		}
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		dprintf(pipe_fd[1], "%s\n", line);
		free(line);
	}
	close(pipe_fd[1]);
	if (readline_interrupted)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	return (pipe_fd[0]);
}
