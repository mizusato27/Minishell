#include "minishell.h"

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
