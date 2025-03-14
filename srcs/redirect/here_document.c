/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_document.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 22:43:20 by mizusato          #+#    #+#             */
/*   Updated: 2025/03/14 15:49:16 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	put_heredoc_line(char *line, int fd)
{
	ft_putstr_fd(line, fd);
	ft_putchar_fd('\n', fd);
}

static int	is_fin_process(char *line, const char *delim, int flag)
{
	if (line == NULL)
		return (1);
	else if (flag == 1)
		return (1);
	else if (ft_strcmp(line, delim) == 0)
		return (1);
	return (0);
}

static char	*expand_here_document(char *line, bool is_quoted)
{
	char	*new_str;
	char	*ptr;

	if (is_quoted)
		return (line);
	ptr = line;
	new_str = ft_calloc(1, sizeof(char));
	if (!new_str)
		malloc_error(ER_CALLOC);
	while (*ptr)
	{
		if (is_variable(ptr))
			expand_var_str(&new_str, &ptr, ptr);
		else if (is_special_param(ptr))
			expand_special_param_str(&new_str, &ptr, ptr);
		else
			add_char(&new_str, *ptr++);
	}
	free(line);
	return (new_str);
}

int	read_here_document(const char *delimiter, bool is_quoted, int flag)
{
	int		pipe_fd[2];
	char	*line;
	bool	rl_intr;

	ft_pipe(pipe_fd);
	rl_intr = false;
	while (1)
	{
		line = readline("> ");
		if (is_fin_process(line, delimiter, flag))
		{
			free(line);
			break ;
		}
		line = expand_here_document(line, is_quoted);
		put_heredoc_line(line, pipe_fd[1]);
		free(line);
	}
	ft_close(pipe_fd[1]);
	if (rl_intr)
	{
		ft_close(pipe_fd[0]);
		return (-1);
	}
	return (pipe_fd[0]);
}
