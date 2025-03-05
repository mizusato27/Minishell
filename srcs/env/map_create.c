/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_create.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 12:34:43 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/05 17:21:36 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

//item_newは新しいアイテムを作成する関数
t_item	*item_new(char *name, char *value)
{
	t_item	*item;

	item = ft_calloc(1, sizeof(*item));
	if (item == NULL)
		malloc_error(ER_MALLOC_CALLOC);
	item->name = name;
	item->value = value;
	return (item);
}

//map_newは新しいマップを作成する関数
t_map	*map_new(void)
{
	t_map	*map;

	map = ft_calloc(1, sizeof(*map));
	if (map == NULL)
		malloc_error(ER_MALLOC_CALLOC);
	return (map);
}

// 新しいアイテムを作成してマップに追加する関数
static void	map_add_item(t_map *map, const char *name, const char *value)
{
	t_item *new_item;

	if (value == NULL)
		new_item = item_new(ft_strdup(name), NULL);
	else
		new_item = item_new(ft_strdup(name), ft_strdup(value));

	if (new_item->name == NULL || (value != NULL && new_item->value == NULL))
		malloc_error(ER_MALLOC_STRDUP);

	new_item->next = map->item_head.next;
	map->item_head.next = new_item;
}

//map_update_itemはアイテムの値を更新する関数
static void	map_update_item(t_item *item, const char *value)
{
	free(item->value);
	if (value == NULL)
		item->value = NULL;
	else
	{
		item->value = ft_strdup(value);
		if (item->value == NULL)
			malloc_error(ER_MALLOC_STRDUP);
	}
}

//直接キーと値を設定する関数
//キーが存在しない場合は新しいアイテムを作成し、キーが存在する場合は値を更新する
int	map_set(t_map *map, const char *name, const char *value)
{
	t_item	*cur;

	if (name == NULL || !is_identifier(name))
		return (-1);
	cur = map->item_head.next;
	while (cur)
	{
		if (ft_strcmp(cur->name, name) == 0)
			break ;
		cur = cur->next;
	}
	if (cur)
		map_update_item(cur, value);
	else
		map_add_item(map, name, value);
	return (EXIT_SUCCESS);
}
