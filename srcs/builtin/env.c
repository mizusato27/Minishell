/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 16:57:52 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/14 11:18:03 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_under(char c)
{
	if (c == '_')
		return (true);
	return (false);
}

static void	print_env(t_item *cursor)
{
	if (!cursor)
		return ;
	if (cursor)
		print_env(cursor->next);
	if (ft_strlen(cursor->name) == 1 && is_under(*(cursor->name)))
		return ;
	if (cursor->value)
		printf("%s=%s\n", cursor->name, cursor->value);
}

int	builtin_env(void)
{
	t_item	*cursor;

	cursor = g_envmap->item_head.next;
	print_env(cursor);
	printf("_=/usr/bin/env\n");
	return (0);
}
