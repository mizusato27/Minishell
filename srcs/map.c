/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 23:35:00 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/19 17:47:41 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

bool	is_identifier(const char *s);
bool	is_alpha_under(char c);
bool	is_alpha_num_under(char c);

//is_alpha_underはアルファベットかアンダースコアかどうかを判定する関数
bool	is_alpha_under(char c)
{
	return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_');
}

//is_alpha_num_underはアルファベットか数字かアンダースコアかどうかを判定する関数
bool is_alpha_num_under(char c)
{
	return (is_alpha_under(c) || ('0' <= c && c <= '9'));
}

//is_identifierは文字列が識別子かどうかを判定する関数
bool	is_identifier(const char *s)
{
	if (!is_alpha_under(*s))
		return (false);
	s++;
	while (*s)
	{
		if (!is_alpha_num_under(*s))
			return (false);
		s++;
	}
	return (true);
}

//item_newは新しいアイテムを作成する関数
t_item	*item_new(char *name, char *value)
{
	t_item	*item;

	item = ft_calloc(1, sizeof(*item));
	if (item == NULL)
		error("calloc");
	item->name = name;
	item->value = value;
	return (item);
}

//item_get_stringはアイテムを文字列に変換する関数
char	*item_get_string(t_item *item)
{
	size_t	strsize;
	char	*string;

	strsize = ft_strlen(item->name) + 2;
	if (item->value)
		strsize += ft_strlen(item->value);
	string = malloc(strsize);
	if (string == NULL)
		error("malloc");
	ft_strlcpy(string, item->name, strsize);
	if (item->value)
	{
		ft_strlcat(string, "=", strsize);
		ft_strlcat(string, item->value, strsize);
	}
	return (string);
}

//map_newは新しいマップを作成する関数
t_map	*map_new(void)
{
	t_map	*map;

	map = ft_calloc(1, sizeof(*map));
	if (map == NULL)
		error(ER_MALLOC_CALLOC);
	return (map);
}

//map_getはマップから値を取得する関数
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

//map_unsetはマップから値を削除する関数
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

//map_setはマップに値を設定する関数
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
	// found
	if (cur)
	{
		free(cur->value);
		if (value == NULL)
			cur->value = NULL;
		else
		{
			cur->value = ft_strdup(value);
			if (cur->value == NULL)
				error("map_set strdup");
		}
	}
	// not found
	else
	{
		if (value == NULL)
		{
			cur = item_new(strdup(name), NULL);
			if (cur->name == NULL)
				error("strdup");
		}
		else
		{
			cur = item_new(strdup(name), ft_strdup(value));
			if (cur->name == NULL || cur->value == NULL)
				error("strdup");
		}
		cur->next = map->item_head.next;
		map->item_head.next = cur;
	}
	return (0);
}

//map_putはマップに値を設定する関数
int	map_put(t_map *map, const char *string, bool allow_empty_value)
{
	int		result;
	char	*name_end;
	char	*name;
	char	*value;
	size_t	name_len;

	name_end = ft_strchr(string, '=');
	// value is null
	if (name_end == NULL)
	{
		if (!allow_empty_value)
			return (-1);
		name = ft_strdup(string);
		value = NULL;
		if (name == NULL)
			error("strdup");
	}
	// value is non null
	else
	{
		name_len = name_end - string;
		name = ft_calloc(name_len + 1, 1);
		if (name == NULL)
			error("calloc");
		ft_strlcpy(name, string, name_len + 1);
		name[name_len] = '\0';
		value = ft_strdup(name_end + 1);
		if (name == NULL || value == NULL)
			error("strdup");
	}
	result = map_set(map, name, value);
	free(name);
	free(value);
	return (result);
}

//map_lenはマップの長さを取得する関数
size_t	map_len(t_map *map, bool count_null_value)
{
	size_t	len;
	t_item	*item;

	len = 0;
	item = map->item_head.next;
	while (item)
	{
		if (item->value || count_null_value)
			len++;
		item = item->next;
	}
	return (len);
}