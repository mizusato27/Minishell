/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 19:56:55 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/27 16:38:21 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
