/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:19:44 by mizusato          #+#    #+#             */
/*   Updated: 2025/03/04 21:16:24 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_tok(t_token **tokens, t_token *tok)
{
	if (*tokens == NULL)
	{
		*tokens = tok;
		return ;
	}
	append_tok(&(*tokens)->next, tok);
}

bool	at_eof(t_token *tok)
{
	if (tok->kind == TK_EOF)
		return (true);
	return (false);
}

t_token	*tokdup(t_token *tok)
{
	char	*word;

	word = ft_strdup(tok->word);
	if (word == NULL)
		error(ER_MALLOC_STRDUP);
	return (new_token(word, tok->kind));
}

bool	equal_operators(t_token *tok, char *op)
{
	if (tok->kind != TK_OP)
		return (false);
	if (ft_strcmp(tok->word, op) == 0)
		return (true);
	return (false);
}
