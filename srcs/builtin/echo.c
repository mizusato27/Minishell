/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 15:37:03 by mizusato          #+#    #+#             */
/*   Updated: 2025/02/27 15:37:06 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_n_option(char *arg)
{
	if (!arg)
		return (0);
	if (ft_strncmp(arg, "-n", 2) == 0)
		return (1);
	return (0);
}

int	builtin_echo(char **argv)
{
	int	n_option_flag;
	int	first_arg_flag;
	int	i;

	i = 1;
	n_option_flag = check_n_option(argv[1]);
	if (n_option_flag == 1)
		i++;
	first_arg_flag = 1;
	while (argv[i])
	{
		if (first_arg_flag == 0)
			ft_putchar_fd(' ', STDOUT_FILENO);
		first_arg_flag = 0;
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		i++;
	}
	if (n_option_flag == 0)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
