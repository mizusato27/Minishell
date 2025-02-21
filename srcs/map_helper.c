/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 09:51:24 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/20 12:05:10 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

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
		error(ER_MALLOC_CALLOC);
	item->name = name;
	item->value = value;
	return (item);
}
