/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:05:30 by mizusato          #+#    #+#             */
/*   Updated: 2025/03/05 18:55:29 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*parse_redir_out(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_REDIR_OUT);
	node->filename = token_dup(tok->next);
	node->targetfd = STDOUT_FILENO;
	*rest = tok->next->next;
	return (node);
}

t_node	*parse_redir_in(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_REDIR_IN);
	node->filename = token_dup(tok->next);
	node->targetfd = STDIN_FILENO;
	*rest = tok->next->next;
	return (node);
}

t_node	*parse_redir_append(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_REDIR_APPEND);
	node->filename = token_dup(tok->next);
	node->targetfd = STDOUT_FILENO;
	*rest = tok->next->next;
	return (node);
}

t_node	*parse_redir_heredoc(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_REDIR_HEREDOC);
	node->delimiter = token_dup(tok->next);
	node->is_delim_quoted = false;// <--- todo:ヒアドキュメントの展開の有無
	node->targetfd = STDIN_FILENO;
	*rest = tok->next->next;
	return (node);
}
