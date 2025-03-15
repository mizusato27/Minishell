/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 23:43:43 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/15 16:46:01 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_cmd_elm(t_node *command, t_token **rest, t_token *tok,
		bool *syntax_error)
{
	if (tok->kind == TK_WORD)
	{
		add_token(&command->args, token_dup(tok));
		tok = tok->next;
	}
	else if (equal_operators(tok, ">") && tok->next->kind == TK_WORD)
		add_node(&command->redirects, parse_redir_out(&tok, tok));
	else if (equal_operators(tok, "<") && tok->next->kind == TK_WORD)
		add_node(&command->redirects, parse_redir_in(&tok, tok));
	else if (equal_operators(tok, ">>") && tok->next->kind == TK_WORD)
		add_node(&command->redirects, parse_redir_append(&tok, tok));
	else if (equal_operators(tok, "<<") && tok->next->kind == TK_WORD)
		add_node(&command->redirects, parse_redir_heredoc(&tok, tok));
	else
		parse_error(ER_ADD_CMD, &tok, tok, syntax_error);
	*rest = tok;
}

static t_node	*process_simple_cmd(t_token **rest, t_token *tok,
		bool *syntax_error)
{
	t_node	*node;

	node = new_node(ND_SIMPLE_CMD);
	add_cmd_elm(node, &tok, tok, syntax_error);
	while (tok && !at_eof(tok) && !is_ctrl_operator(tok))
		add_cmd_elm(node, &tok, tok, syntax_error);
	*rest = tok;
	return (node);
}

static t_node	*parse_pipeline(t_token **rest, t_token *tok,
		bool *syntax_error)
{
	t_node	*node;

	node = new_node(ND_PIPELINE);
	node->inpipe[0] = STDIN_FILENO;
	node->inpipe[1] = -1;
	node->outpipe[0] = -1;
	node->outpipe[1] = STDOUT_FILENO;
	node->command = process_simple_cmd(&tok, tok, syntax_error);
	if (equal_operators(tok, "|"))
		node->next = parse_pipeline(&tok, tok->next, syntax_error);
	*rest = tok;
	return (node);
}

t_node	*parse(t_token *tok, bool *syntax_error)
{
	t_node	*node;

	node = parse_pipeline(&tok, tok, syntax_error);
	return (node);
}
