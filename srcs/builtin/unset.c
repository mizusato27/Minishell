/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 21:57:44 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/15 15:17:05 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	envmap_unset(t_map *envmap, const char *name)
{
	t_item	*current;
	t_item	*prev;

	if (name == NULL)
		return (ERROR_UNSET);
	prev = &envmap->item_head;
	current = envmap->item_head.next;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			prev->next = current->next;
			free(current->name);
			free(current->value);
			free(current);
			return (EXIT_SUCCESS);
		}
		prev = prev->next;
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

int	builtin_unset(t_map *envmap, char **argv)
{
	int		status;
	size_t	i;

	status = 0;
	i = 1;
	while (argv[i])
	{
		if (envmap_unset(envmap, argv[i]) == ERROR_UNSET)
		{
			builtin_error("unset", argv[i], "not a valid identifier");
			status = 1;
		}
		else
			status = 0;
		i++;
	}
	return (status);
}
