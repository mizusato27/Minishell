/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 20:47:37 by mizusato          #+#    #+#             */
/*   Updated: 2024/04/16 21:08:50 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	else if (n < 0)
	{
		write(fd, "-", 1);
		n *= (-1);
	}
	if (n > 9)
		ft_putnbr_fd((n / 10), fd);
	write(fd, &"0123456789"[n % 10], 1);
}
/*
#include <stdlib.h>
int	main(int argc, char **argv)
{
	(void)argc;
	ft_putnbr_fd(atoi(argv[1]), 1);
	write(1, "\n", 1);
	return (0);
}
*/
