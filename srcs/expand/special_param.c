/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_param.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 15:38:49 by mizusato          #+#    #+#             */
/*   Updated: 2025/02/27 15:38:51 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_param(char *str)
{
	if (str[0] == '$' && str[1] == '?')
		return (1);
	return (0);
}

static void	add_number(char **dst, unsigned int n)
{
	if (n == 0)
	{
		add_char(dst, '0');
		return ;
	}
	if (n / 10 != 0)
		add_number(dst, n / 10);
	add_char(dst, '0' + (n % 10));
}

void	expand_special_param_str(char **dst, char **rest, char *ptr)
{
	if (!is_special_param(ptr))
		assert_error("Expected special parameter");
	ptr += 2;
	add_number(dst, last_status);
	*rest = ptr;
}
