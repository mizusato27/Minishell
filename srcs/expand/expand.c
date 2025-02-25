/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 00:28:56 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/25 11:26:29 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// getenvが出てきたら、xgetenvに変更

void	todo(char *message)
{
	printf("TODO: %s\n", message);
}

//文字列の最後に文字を追加
void	add_char(char **s, char c)
{
	size_t	size;
	char	*new;

	size = APPEND_CHAR_SIZE + END_CHAR_SIZE;
	if (*s)
		size += ft_strlen(*s);
	new = malloc(size);
	if (new == NULL)
		error(ER_MALLOC);
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

// int main()
// {

// 	return (0);
// }
