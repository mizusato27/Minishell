/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:23:43 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/04 14:51:39 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	err_exit(const char *location, const char *msg, int status)
{
	printf("minishell: %s: %s\n", location, msg);
	exit(status);
}

void	error(char *msg)
{
	printf("%s\n", msg);
	exit(2);
}

void    malloc_error()
{
    printf("malloc error\n");
    exit(1);
}



