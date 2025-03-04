/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 09:51:24 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/02 18:02:45 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

//is_alpha_underはアルファベットかアンダースコアかどうかを判定する関数
bool	is_alpha_under(char c)
{
	return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_');
}

//is_alpha_num_underはアルファベットか数字かアンダースコアかどうかを判定する関数
bool	is_alpha_num_under(char c)
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

//map_lenはマップの長さを取得する関数
size_t	map_len(t_map *map, bool count_null_value)
{
	size_t	len;
	t_item	*item;

	len = 0;
	item = map->item_head.next;
	while (item)
	{
		if (item->value || count_null_value)
			len++;
		item = item->next;
	}
	return (len);
}

