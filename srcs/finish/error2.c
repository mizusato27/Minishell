/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:07:48 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/14 10:34:31 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	malloc_error(char *msg)
{
	perror_prefix();
	write(STDERR_FILENO, "malloc error: ", 14);
	write(STDERR_FILENO, msg, ft_strlen(msg));
	exit(2);
}

void	fatal_error(const char *msg)
{
	perror_prefix();
	write(STDERR_FILENO, "Fatal Error: ", 13);
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	exit(1);
}

void	assert_error(const char *msg)
{
	perror_prefix();
	write(STDERR_FILENO, "Assert Error: ", 14);
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	exit(255);
}

void	xperror(const char *location)
{
	perror_prefix();
	perror(location);
}
