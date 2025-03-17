/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 00:32:20 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/17 13:52:07 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_metacharacter(char c)
{
	if (is_blank(c))
		return (true);
	return (c && ft_strchr("|<> \t\n", c));
}

bool	which_op(const char *s, const char *op)
{
	return (ft_memcmp(s, op, ft_strlen(op)) == 0);
}

bool	is_operator(const char *s)
{
	char	*operators[7];
	int		i;

	init_operators(operators);
	i = 0;
	while (operators[i])
	{
		if (which_op(s, operators[i]))
			return (true);
		i++;
	}
	return (false);
}

void	init_operators(char *operators[7])
{
	operators[0] = "|";
	operators[1] = "\n";
	operators[2] = "<<";
	operators[3] = ">>";
	operators[4] = "<";
	operators[5] = ">";
	operators[6] = NULL;
}

t_token	*new_token(char *word, t_token_kind kind)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(*token));
	if (token == NULL)
		malloc_error(ER_CALLOC);
	token->word = word;
	token->kind = kind;
	return (token);
}
