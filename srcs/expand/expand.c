/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 00:28:56 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/05 17:22:12 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	todo(char *message)
{
	printf("TODO: %s\n", message);
}

void	add_char(char **s, char c)
{
	size_t	size;
	char	*new;

	size = APPEND_CHAR_SIZE + END_CHAR_SIZE;
	if (*s)
		size += ft_strlen(*s);
	new = malloc(size);
	if (new == NULL)
		malloc_error(ER_MALLOC);
	if (*s)
		ft_strlcpy(new, *s, size);
	new[size - 2] = c;
	new[size - 1] = '\0';
	if (*s)
		free(*s);
	*s = new;
}

void	expand(t_node *node)
{
	expand_variable(node);
	expand_quote_removal(node);
}
