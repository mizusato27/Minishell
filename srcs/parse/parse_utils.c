/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:32:58 by mizusato          #+#    #+#             */
/*   Updated: 2025/03/08 18:26:23 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_ctrl_operators(char *operators[2])
{
	operators[0] = "|";
	operators[1] = "\n";
}

static int	cmp_prefix(const char *s, const char *keyword)
{
	int	result;

	result = ft_memcmp(s, keyword, ft_strlen(keyword));
	return (result);
}

bool	is_ctrl_operator(t_token *tok)
{
	char	*operators[2];
	size_t	i;

	i = 0;
	init_ctrl_operators(operators);
	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (cmp_prefix(tok->word, operators[i]) == 0)
			return (true);
		i++;
	}
	return (false);
}

t_node	*new_node(t_node_kind kind)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(*node));
	if (node == NULL)
		malloc_error(ER_CALLOC);
	node->kind = kind;
	return (node);
}

void	add_node(t_node **node, t_node *elm)
{
	if (*node == NULL)
	{
		*node = elm;
		return ;
	}
	add_node(&(*node)->next, elm);
}
