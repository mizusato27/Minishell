/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_get_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 02:14:10 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/17 13:50:16 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*map_get_value(t_map *map, const char *name)
{
	t_item	*cur;

	if (name == NULL)
		return (NULL);
	cur = map->item_head.next;
	while (cur)
	{
		if (ft_strcmp(cur->name, name) == 0)
			return (cur->value);
		cur = cur->next;
	}
	return (NULL);
}
