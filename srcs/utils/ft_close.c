/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_close.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 02:05:52 by mizusato          #+#    #+#             */
/*   Updated: 2025/03/01 02:12:25 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_close(int fildes)
{
	if (fildes < 0)
		fatal_error(ER_FILE);
	if (close(fildes) < 0)
		fatal_error(ER_CLOSE);
	return (0);
}
