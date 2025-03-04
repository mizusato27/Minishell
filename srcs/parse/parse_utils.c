/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:32:58 by mizusato          #+#    #+#             */
/*   Updated: 2025/03/04 21:51:44 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_ctrl_operators(char *operators[2])
{
	operators[0] = "|";
	operators[1] = "\n";
}

static bool	startswith(const char *s, const char *keyword)
{
	if (ft_memcmp(s, keyword, ft_strlen(keyword)) == 0)
		return (true);
	return (false);
}

bool	is_ctrl_operator(t_token *tok)
{
	char	*operators[2];
	size_t	i;

	i = 0;
	init_ctrl_operators(operators);
	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (startswith(tok->word, operators[i]))
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
		error(ER_MALLOC_CALLOC);
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
