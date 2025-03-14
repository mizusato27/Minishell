/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 22:43:07 by mizusato          #+#    #+#             */
/*   Updated: 2025/03/14 14:29:45 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_open_file(t_node *node, int *status)
{
	int	fd;

	fd = 0;
	if (node->kind == ND_REDIR_OUT)
		fd = open(node->filename->word, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (node->kind == ND_REDIR_IN)
		fd = open(node->filename->word, O_RDONLY);
	else if (node->kind == ND_REDIR_APPEND)
		fd = open(node->filename->word, O_CREAT | O_WRONLY | O_APPEND,
				0644);
	else if (node->kind == ND_REDIR_HEREDOC)
		fd = read_here_document(node->delimiter->word, node->is_delim_quoted, status);
	else
		assert_error(ER_FILE);
	if (fd < 0)
	{
		if (node->kind == ND_REDIR_OUT || node->kind == ND_REDIR_APPEND
			|| node->kind == ND_REDIR_IN)
			xperror(node->filename->word);
	}
	return (fd);
}

int	open_redirect_file(t_node *node, int *status)
{
	if (node == NULL)
		return (0);
	if (node->kind == ND_PIPELINE)
	{
		if (open_redirect_file(node->command, status) < 0)
			return (-1);
		if (open_redirect_file(node->next, status) < 0)
			return (-1);
		return (0);
	}
	else if (node->kind == ND_SIMPLE_CMD)
		return (open_redirect_file(node->redirects, status));
	node->filefd = process_open_file(node, status);
	if (node->filefd < 0)
		return (-1);
	node->filefd = stash_fd(node->filefd);
	return (open_redirect_file(node->next, status));
}
