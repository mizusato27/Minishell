/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 23:35:03 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/17 13:50:13 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static	t_map	*map_new(void)
{
	t_map	*map;

	map = ft_calloc(1, sizeof(*map));
	if (map == NULL)
		malloc_error(ER_CALLOC);
	return (map);
}

t_map	*initenv(void)
{
	extern char	**environ;
	t_map		*envmap;

	envmap = map_new();
	envmap_init(envmap, environ);
	return (envmap);
}
