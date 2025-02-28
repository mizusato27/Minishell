/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dup2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 02:00:57 by mizusato          #+#    #+#             */
/*   Updated: 2025/03/01 02:04:53 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_dup2(int fildes, int fildes2)
{
	int	fd;

	fd = dup2(fildes, fildes2);
	if (fd < 0)
		fatal_error(ER_DUP2);
	return (fd);
}
