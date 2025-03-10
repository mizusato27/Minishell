/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 16:58:17 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/07 23:19:40 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	process_sign(char *s, int *i, int *sign)
{
	if (s[0] == '-' || s[0] == '+')
	{
		if (s[0] == '-')
			*sign = -1;
		else
			*sign = 1;
		(*i)++;
		return (true);
	}
	*sign = 1;
	return (true);
}

static bool	convert_to_long(char *s, int i, int sign, long *result)
{
	unsigned long	res;

	res = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (false);
		res = res * 10 + (s[i] - '0');
		if (res > (unsigned long)LLONG_MAX + (sign == -1))
			return (false);
		i++;
	}
	*result = sign * res;
	return (true);
}

static bool	string_to_long(char *s, long *result)
{
	int	i;
	int	sign;

	i = 0;
	if (!s[0])
		return (false);
	if (!process_sign(s, &i, &sign))
		return (false);
	return (convert_to_long(s, i, sign, result));
}

int	builtin_exit(char **argv)
{
	long	res;
	char	*num;

	if (argv[1] == NULL)
		exit(g_ctx.g_status);
	if (argv[2])
	{
		xperror("exit: too many arguments");
		return (EXIT_FAILURE);
	}
	num = argv[1];
	if (string_to_long(num, &res))
		exit((int)res);
	xperror("exit: numeric argument required");
	exit(2);
}
