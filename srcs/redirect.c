/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 19:56:55 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/27 15:28:35 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool	readline_interrupted = false;

int	stash_fd(int fd)
{
	int	stash;

	stash = fcntl(fd, F_DUPFD, 10);
	if (stash < 0)
		fatal_error("");
	if (close(fd) < 0)
		fatal_error("");
	return (stash);
}

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

int	open_redirect_file(t_node *node)
{
	if (node == NULL)
		return (0);
	if (node->kind == ND_PIPELINE)
	{
		if (open_redirect_file(node->command) < 0)
			return (-1);
		if (open_redirect_file(node->next) < 0)
			return (-1);
		return (0);
	}
	else if (node->kind == ND_SIMPLE_CMD)
		return (open_redirect_file(node->redirects));
	if (node->kind == ND_REDIR_OUT)
		node->filefd = open(node->filename->word, O_CREAT | O_WRONLY | O_TRUNC,
				0644);
	else if (node->kind == ND_REDIR_IN)
		node->filefd = open(node->filename->word, O_RDONLY);
	else if (node->kind == ND_REDIR_APPEND)
		node->filefd = open(node->filename->word, O_CREAT | O_WRONLY | O_APPEND,
				0644);
	else if (node->kind == ND_REDIR_HEREDOC)
		node->filefd = read_here_document(node->delimiter->word);
	else
		assert_error("open_redir_file");
	if (node->filefd < 0)
	{
		if (node->kind == ND_REDIR_OUT || node->kind == ND_REDIR_APPEND || node->kind == ND_REDIR_IN)//<-signal.c
			xperror(node->filename->word);
		return (-1);
	}
	node->filefd = stash_fd(node->filefd);
	return (open_redirect_file(node->next));
}

void	do_redirect(t_node *redirects)
{
	if (redirects == NULL)
		return ;
	if (redirects->kind == ND_REDIR_OUT || redirects->kind == ND_REDIR_IN
		|| redirects->kind == ND_REDIR_APPEND
		|| redirects->kind == ND_REDIR_HEREDOC)
	{
		redirects->stashed_targetfd = stash_fd(redirects->targetfd);
		dup2(redirects->filefd, redirects->targetfd);
	}
	else
		assert_error("do_redirect");
	do_redirect(redirects->next);
}

void	reset_redirect(t_node *redirects)
{
	if (redirects == NULL)
		return ;
	reset_redirect(redirects->next);
	if (redirects->kind == ND_REDIR_OUT || redirects->kind == ND_REDIR_IN
		|| redirects->kind == ND_REDIR_APPEND
		|| redirects->kind == ND_REDIR_HEREDOC)
	{
		close(redirects->filefd);
		close(redirects->targetfd);
		dup2(redirects->stashed_targetfd, redirects->targetfd);
	}
	else
		assert_error("reset_redirect");
}
