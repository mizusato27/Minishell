/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stash_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 22:43:13 by mizusato          #+#    #+#             */
/*   Updated: 2025/03/14 10:08:35 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_fd(int fd)
{
	struct stat	st;

	if (fd < 0)
		return (0);
	errno = 0;
	if (fstat(fd, &st) < 0)
		return (0);
	return (1);
}

static int	available_fd(void)
{
	int	fd;

	fd = 10;
	while (is_valid_fd(fd))
		fd++;
	return (fd);
}

int	stash_fd(int fd)
{
	int	new_fd;

	new_fd = available_fd();
	if (!is_valid_fd(fd))
	{
		errno = EBADF;
		return (-1);
	}
	new_fd = ft_dup2(fd, new_fd);
	printf("");
	ft_close(fd);
	return (new_fd);
}
