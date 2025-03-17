/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_document.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 22:43:20 by mizusato          #+#    #+#             */
/*   Updated: 2025/03/17 10:57:18 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	put_heredoc_line(char *line, int fd)
{
	ft_putstr_fd(line, fd);
	ft_putchar_fd('\n', fd);
}

static int	is_fin_process(char *line, const char *delim, bool *rl_intr)
{
	if (line == NULL)
		return (1);
	else if (*rl_intr)
		return (1);
	else if (ft_strcmp(line, delim) == 0)
		return (1);
	return (0);
}

static char	*expand_here_document(t_map *envmap, char *line, bool is_quoted,
		int *status)
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
			expand_var_str(envmap, &new_str, &ptr, ptr);
		else if (is_special_param(ptr))
			expand_special_param_str(&new_str, &ptr, ptr, status);
		else
			add_char(&new_str, *ptr++);
	}
	free(line);
	return (new_str);
}

int	read_here_document(t_map *envmap, const char *delimiter, bool is_quoted,
		int *status)
{
	int		pipe_fd[2];
	char	*line;
	bool	rl_intr;

	ft_pipe(pipe_fd);
	rl_intr = false;
	while (1)
	{
		line = readline("> ");
		if (g_sig == SIGINT)
			rl_intr = true;
		if (is_fin_process(line, delimiter, &rl_intr))
		{
			free(line);
			break ;
		}
		line = expand_here_document(envmap, line, is_quoted, status);
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
