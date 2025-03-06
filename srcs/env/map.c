/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 23:35:00 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/06 18:17:38 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// map_getはマップから値を取得する関数
char	*map_get(t_map *map, const char *name)
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

// map_unsetはマップから値を削除する関数
int	map_unset(t_map *map, const char *name)
{
	t_item	*cur;
	t_item	*prev;

	if (name == NULL || !is_identifier(name))
		return (-1);
	prev = &map->item_head;
	cur = map->item_head.next;
	while (cur)
	{
		if (ft_strcmp(cur->name, name) == 0)
		{
			prev->next = cur->next;
			free(cur->name);
			free(cur->value);
			free(cur);
			return (0);
		}
		prev = prev->next;
		cur = cur->next;
	}
	return (0);
}

//"KEY=VALUE" の形式の文字列を受け取り、キーと値を分割して設定する関数
static void	handle_no_equal(char **name, char **value, const char *string,
		bool allow_empty_value)
{
	if (!allow_empty_value)
	{
		*name = NULL;
		*value = NULL;
		return ;
	}
	*name = ft_strdup(string);
	*value = NULL;
	if (*name == NULL)
		malloc_error(ER_MALLOC_STRDUP);
}

static void	handle_with_equal(char **name, char **value, const char *string,
		char *name_end)
{
	size_t	name_len;

	name_len = name_end - string;
	*name = ft_calloc(name_len + 1, 1);
	if (*name == NULL)
		malloc_error(ER_MALLOC_CALLOC);
	ft_strlcpy(*name, string, name_len + 1);
	(*name)[name_len] = '\0';
	*value = ft_strdup(name_end + 1);
	if (*name == NULL || *value == NULL)
		malloc_error(ER_MALLOC_STRDUP);
}

int	map_put(t_map *map, const char *string, bool allow_empty_value)
{
	char	*name;
	char	*value;
	int		result;
	char	*name_end;

	name_end = ft_strchr(string, '=');
	if (name_end == NULL)
		handle_no_equal(&name, &value, string, allow_empty_value);
	else
		handle_with_equal(&name, &value, string, name_end);
	if (name == NULL)
		return (-1);
	result = map_set(map, name, value);
	free(name);
	free(value);
	return (result);
}

// int	map_put(t_map *map, const char *string, bool allow_empty_value)
// {
// 	int		result;
// 	char	*name_end;
// 	char	*name;
// 	char	*value;
// 	size_t	name_len;

// 	name_end = ft_strchr(string, '=');
// 	if (name_end == NULL)
// 	{
// 		if (!allow_empty_value)
// 			return (-1);
// 		name = ft_strdup(string);
// 		value = NULL;
// 		if (name == NULL)
// 			error(ER_MALLOC_STRDUP);
// 	}
// 	else
// 	{
// 		name_len = name_end - string;
// 		name = ft_calloc(name_len + 1, 1);
// 		if (name == NULL)
// 			error(ER_MALLOC_CALLOC);
// 		ft_strlcpy(name, string, name_len + 1);
// 		name[name_len] = '\0';
// 		value = ft_strdup(name_end + 1);
// 		if (name == NULL || value == NULL)
// 			error(ER_MALLOC_STRDUP);
// 	}
// 	result = map_set(map, name, value);
// 	free(name);
// 	free(value);
// 	return (result);
// }
