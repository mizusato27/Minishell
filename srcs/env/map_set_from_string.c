/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_set_from_string.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 23:35:00 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/14 10:33:01 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_no_equal(char **name, char **value, const char *string,
		bool allow_empty_value)
{
	*value = NULL;
	if (!allow_empty_value)
	{
		*name = NULL;
		return ;
	}
	else
	{
		*name = ft_strdup(string);
		*value = NULL;
		if (*name == NULL)
			malloc_error(ER_STRDUP);
	}
}

static void	handle_with_equal(char **name, char **value, const char *string,
		char *name_end)
{
	size_t	name_len;

	name_len = name_end - string;
	*name = ft_calloc(name_len + 1, 1);
	if (*name == NULL)
		malloc_error(ER_CALLOC);
	ft_strlcpy(*name, string, name_len + 1);
	(*name)[name_len] = '\0';
	*value = ft_strdup(name_end + 1);
	if (*name == NULL || *value == NULL)
		malloc_error(ER_STRDUP);
}

int	map_set_from_string(t_map *map, const char *string, bool allow_empty_value)
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
	result = map_set_value(map, name, value);
	free(name);
	free(value);
	return (result);
}
