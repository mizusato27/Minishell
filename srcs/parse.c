/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 23:43:43 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/11 19:30:40 by ynihei           ###   ########.fr       */
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

t_node	*parse(t_token *tok)
{
	t_node	*node;

	node = new_node(ND_SIMPLE_CMD);
	while (tok && !at_eof(tok))
	{
		// printf("tok:%d\n",tok->kind);
		if (tok->kind == TK_WORD)
		{
			append_tok(&node->args, tokdup(tok));
			tok = tok->next;
		}
		else
			parse_error("Unexpected Token", &tok, tok);
	}
	return (node);
}

