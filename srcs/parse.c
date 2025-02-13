/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 23:43:43 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/13 15:46:09 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void	append_tok(t_token **tokens, t_token *tok)
{
	if (*tokens == NULL)
	{
		*tokens = tok;
		return ;
	}
	append_tok(&(*tokens)->next, tok);
}

t_node	*new_node(t_node_kind kind)
{
	t_node	*node;

	node = calloc(1, sizeof(*node));
	if (node == NULL)
		error(ER_MALLOC_CALLOC);
	node->kind = kind;
	return (node);
}

bool	at_eof(t_token *tok)
{
	return (tok->kind == TK_EOF);
}

t_token	*tokdup(t_token *tok)
{
	char	*word;

	word = ft_strdup(tok->word);
	if (word == NULL)
		error(ER_MALLOC_STRDUP);
	return (new_token(word, tok->kind));
}

// 追加部分
#include <string.h>

bool	equal_op(t_token *tok, char *op)
{
	if (tok->kind != TK_OP)
		return (false);
	return (strcmp(tok->word, op) == 0);
}

void	append_node(t_node **node, t_node *elm)
{
	if (*node == NULL)
	{
		*node = elm;
		return ;
	}
	append_node(&(*node)->next, elm);
}

t_node	*redirect_out(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_REDIR_OUT);
	node->filename = tokdup(tok->next);
	node->targetfd = STDOUT_FILENO;
	*rest = tok->next->next;
	return (node);
}

t_node	*redirect_in(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_REDIR_IN);
	node->filename = tokdup(tok->next);
	node->targetfd = STDIN_FILENO;
	*rest = tok->next->next;
	return (node);
}

t_node	*redirect_append(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_REDIR_APPEND);
	node->filename = tokdup(tok->next);
	node->targetfd = STDOUT_FILENO;
	*rest = tok->next->next;
	return (node);
}

t_node	*redirect_heredoc(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_REDIR_HEREDOC);
	node->delimiter = tokdup(tok->next);
	node->targetfd = STDIN_FILENO;
	*rest = tok->next->next;
	return (node);
}

void	append_command_element(t_node *command, t_token **rest, t_token *tok)
{
	if (tok->kind == TK_WORD)
	{
		append_tok(&command->args, tokdup(tok));
		tok = tok->next;
	}
	else if (equal_op(tok, ">") && tok->next->kind == TK_WORD)
		append_node(&command->redirects, redirect_out(&tok, tok));
	else if (equal_op(tok, "<") && tok->next->kind == TK_WORD)
		append_node(&command->redirects, redirect_in(&tok, tok));
	else if (equal_op(tok, ">>") && tok->next->kind == TK_WORD)
		append_node(&command->redirects, redirect_append(&tok, tok));
	else if (equal_op(tok, "<<") && tok->next->kind == TK_WORD)
		append_node(&command->redirects, redirect_heredoc(&tok, tok));
	else
		todo("append_command_element");
	*rest = tok;
}

// 一部修正
t_node	*parse(t_token *tok)
{
	t_node	*node;

	node = new_node(ND_SIMPLE_CMD);
	append_command_element(node, &tok, tok);// <--- 追加
	while (tok && !at_eof(tok))
	// {
	// 	if (tok->kind == TK_WORD)
	// 	{
	// 		append_tok(&node->args, tokdup(tok));
	// 		tok = tok->next;
	// 	}
	// 	else
	// 		parse_error("Unexpected Token", &tok, tok);
	// }
		append_command_element(node, &tok, tok);// <--- 追加
	return (node);
}
