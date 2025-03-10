/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 19:56:55 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/02 16:46:25 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_redirect(t_node_kind kind)
{
	if (kind == ND_REDIR_OUT)
		return (1);
	else if (kind == ND_REDIR_IN)
		return (1);
	else if (kind == ND_REDIR_APPEND)
		return (1);
	else if (kind == ND_REDIR_HEREDOC)
		return (1);
	return (0);
}

void	setup_redirect(t_node *redirects)
{
	if (redirects == NULL)
		return ;
	if (!is_redirect(redirects->kind))
		assert_error(ER_SETUP_REDIR);
	redirects->stashed_targetfd = stash_fd(redirects->targetfd);
	ft_dup2(redirects->filefd, redirects->targetfd);
	setup_redirect(redirects->next);
}

void	reset_redirect(t_node *redirects)
{
	if (redirects == NULL)
		return ;
	if (!is_redirect(redirects->kind))
		assert_error(ER_RESET_REDIR);
	reset_redirect(redirects->next);
	ft_close(redirects->filefd);
	ft_close(redirects->targetfd);
	ft_dup2(redirects->stashed_targetfd, redirects->targetfd);
}
