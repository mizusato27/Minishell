/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:23:43 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/14 14:39:23 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static pid_t	execute_pipe(t_node *node, int *status)
{
	pid_t	pid;

	if (node == NULL)
		return (-1);
	create_new_pipe(node);
	pid = fork();
	if (pid < 0)
		fatal_error("fork");
	else if (pid == 0)
	{
		reset_signal();
		process_child_pipe(node);
		if (is_builtin(node))
			exit(exec_builtin(node, status));
		else
			exec_nonbuiltin(node);
	}
	process_parent_pipe(node);
	if (node->next)
		return (execute_pipe(node->next, status));
	return (pid);
}

static int	wait_pipe(pid_t pid)
{
	pid_t	result;
	int		status;
	int		wstatus;

	while (1)
	{
		result = wait(&wstatus);
		if (result == pid)
		{
			if (WIFSIGNALED(wstatus))
				status = 128 + WTERMSIG(wstatus);
			else
				status = WEXITSTATUS(wstatus);
		}
		else if (result < 0)
		{
			if (errno == ECHILD)
				break ;
			else if (errno == EINTR)
				continue ;
			else
				fatal_error("wait");
		}
	}
	return (status);
}

static int	execute_cmd(t_node *node, int *status)
{
	pid_t	pid;
	int		cmd_status;

	if (open_redirect_file(node, status) < 0)
		return (ERROR_OPEN_REDIR);
	if (node->next == NULL && is_builtin(node))
		cmd_status = exec_builtin(node, status);
	else
	{
		pid = execute_pipe(node, status);
		cmd_status = wait_pipe(pid);
	}
	return (cmd_status);
}

void	interpret_cmd(char *line, int *last_status)
{
	t_token	*tok;
	t_node	*node;

	tok = tokenize(line);
	if (at_eof(tok))
		;
	else if (g_syntax_error)
		*last_status = ERROR_TOKENIZE;
	else
	{
		node = parse(tok);
		if (g_syntax_error)
			*last_status = ERROR_PARSE;
		else
		{
			expand(node, last_status);
			*last_status = execute_cmd(node, last_status);
		}
		free_node(node);
	}
	free_token(tok);
}
