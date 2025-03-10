/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 23:35:03 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/08 18:26:23 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// item_get_stringはアイテムを文字列に変換する関数
// +2 for '=' and '\0'
static	char	*item_connect_equals(t_item *item)
{
	size_t	strsize;
	char	*string;

	strsize = ft_strlen(item->name) + 2;
	if (item->value)
		strsize += ft_strlen(item->value);
	string = malloc(strsize);
	if (string == NULL)
		malloc_error(ER_MALLOC);
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
		malloc_error(ER_CALLOC);
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

// envmap_initは環境変数を初期化する関数
// SHLVLシェルの深さ
static void	envmap_init(t_map *map, char **ep)
{
	char	cwd[PATH_MAX];

	while (*ep)
	{
		map_set_from_string(map, *ep, false);
		ep++;
	}
	if (map_get_value(map, "SHLVL") == NULL)
		map_set_value(map, "SHLVL", "1");
	if (map_get_value(map, "PWD") == NULL)
	{
		getcwd(cwd, PATH_MAX);
		map_set_value(map, "PWD", cwd);
	}
	if (map_get_value(map, "OLDPWD") == NULL)
		map_set_value(map, "OLDPWD", NULL);
}

// map_newは新しいマップを作成する関数
static	t_map	*map_new(void)
{
	t_map	*map;

	map = ft_calloc(1, sizeof(*map));
	if (map == NULL)
		malloc_error(ER_CALLOC);
	return (map);
}

// initenvは環境変数を初期化する関数
// environは環境変数を格納する配列
// externをつけることで、他のファイルからも参照できる
void	initenv(void)
{
	extern char	**environ;

	g_ctx.g_envmap = map_new();
	envmap_init(g_ctx.g_envmap, environ);
}
