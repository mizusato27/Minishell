/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 23:35:03 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/20 12:17:16 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_map		*g_envmap;

//item_get_stringはアイテムを文字列に変換する関数
char	*item_conect_equals(t_item *item)
{
	size_t	strsize;
	char	*string;

	strsize = ft_strlen(item->name) + 2; // +2 for '=' and '\0'
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

// get_environは環境変数を取得する関数
// 環境変数は「変数名=値」の形式で格納されている
char	**get_environ(t_map *map)
{
	size_t	i;
	size_t	size;
	t_item	*item;
	char	**environ;

	size = map_len(map, false) + 1;
	environ = ft_calloc(size, sizeof(char *));
	if (environ == NULL)
		error(ER_MALLOC_CALLOC);
	i = 0;
	item = map->item_head.next;
	while (item)
	{
		if (item->value)
		{
			environ[i] = item_connect_equals(item);
			i++;
		}
		item = item->next;
	}
	environ[i] = NULL;
	return (environ);
}

// xgetenvは環境変数の値を取得する関数
char	*xgetenv(const char *name)
{
	return (map_get(g_envmap, name));
}

// envmap_initは環境変数を初期化する関数
static void	envmap_init(t_map *map, char **ep)
{
	while (*ep)
	{
		map_put(map, *ep, false);
		ep++;
	}
}

// initenvは環境変数を初期化する関数
void	initenv(void)
{
	extern char	**environ;

	g_envmap = ft_calloc(1, sizeof(*g_envmap));
	if (g_envmap == NULL)
		error(ER_MALLOC_CALLOC);
	g_envmap = map_new();
	envmap_init(g_envmap, environ);
}

//map_newは新しいマップを作成する関数
// t_map	*map_new(void)
// {
// 	t_map	*map;

// 	map = ft_calloc(1, sizeof(*map));
// 	if (map == NULL)
// 		error(ER_MALLOC_CALLOC);
// 	return (map);
// }
