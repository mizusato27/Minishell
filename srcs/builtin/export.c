/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 21:11:02 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/15 15:15:34 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	count_env_vars(t_map *envmap)
{
	t_item	*cur;
	size_t	count;

	count = 0;
	cur = envmap->item_head.next;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	return (count);
}

static void	fill_env_array(t_map *envmap, t_item **arr, size_t count)
{
	t_item	*cur;
	size_t	i;

	cur = envmap->item_head.next;
	i = 0;
	while (cur && i < count)
	{
		arr[i] = cur;
		cur = cur->next;
		i++;
	}
}

static void	sort_env(t_item **arr, size_t count)
{
	t_item	*tmp;
	size_t	i;
	size_t	j;

	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(arr[i]->name, arr[j]->name) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_env(t_map *envmap)
{
	t_item	**arr;
	size_t	count;
	size_t	i;

	count = count_env_vars(envmap);
	arr = (t_item **)malloc(sizeof(t_item *) * count);
	if (!arr)
		return ;
	fill_env_array(envmap, arr, count);
	sort_env(arr, count);
	i = 0;
	while (i < count)
	{
		if (ft_strcmp(arr[i]->name, "_") == 0)
		{
			i++;
			continue ;
		}
		printf("declare -x %s", arr[i]->name);
		if (arr[i]->value)
			printf("=\"%s\"", arr[i]->value);
		printf("\n");
		i++;
	}
	free(arr);
}

int	builtin_export(t_map *envmap, char **argv)
{
	size_t	i;
	int		status;

	status = 0;
	i = 1;
	if (argv[i] == NULL)
	{
		print_env(envmap);
		return (status);
	}
	while (argv[i])
	{
		if (map_set_from_string(envmap, argv[i], true) < 0)
		{
			builtin_error("export", argv[i], "not a valid identifier");
			status = 1;
		}
		i++;
	}
	return (status);
}
