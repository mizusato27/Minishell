/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 16:57:52 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/04 12:21:19 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(void)
{
	t_item	*cursor;

	cursor = g_ctx.g_envmap->item_head.next;
	while (cursor)
	{
		if (cursor->value)
			printf("%s=%s\n", cursor->name, cursor->value);
		cursor = cursor->next;
	}
	printf("_=/usr/bin/env\n");
	return (0);
}
