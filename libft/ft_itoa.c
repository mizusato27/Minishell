/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 19:29:20 by mizusato          #+#    #+#             */
/*   Updated: 2024/04/27 15:30:39 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*min_nbr(void)
{
	char	*n;

	n = (char *)malloc(12 * sizeof(char));
	if (n == NULL)
		return (NULL);
	n[0] = '-';
	n[1] = '2';
	n[2] = '1';
	n[3] = '4';
	n[4] = '7';
	n[5] = '4';
	n[6] = '8';
	n[7] = '3';
	n[8] = '6';
	n[9] = '4';
	n[10] = '8';
	n[11] = '\0';
	return (n);
}

static int	count_len(int n)
{
	int	i;

	i = 0;
	while (n != 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

static char	*neg_nbr(int nb)
{
	char	*n;
	int		len;
	int		i;

	nb *= (-1);
	len = count_len(nb);
	n = (char *)malloc((len + 2) * sizeof(char));
	if (n == NULL)
		return (NULL);
	n[0] = '-';
	i = 0;
	while (i < len)
	{
		n[len - i] = nb % 10 + '0';
		nb /= 10;
		i++;
	}
	n[len + 1] = '\0';
	return (n);
}

static char	*pos_nbr(int nb)
{
	char	*n;
	int		len;
	int		i;

	len = count_len(nb);
	n = (char *)malloc((len + 1) * sizeof(char));
	if (n == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		n[len - i - 1] = nb % 10 + '0';
		nb /= 10;
		i++;
	}
	n[len] = '\0';
	return (n);
}

char	*ft_itoa(int n)
{
	char	*nbr;

	if (n == 0)
	{
		nbr = (char *)malloc(2 * sizeof(char));
		if (nbr == NULL)
			return (NULL);
		nbr[0] = '0';
		nbr[1] = '\0';
	}
	else if (n == -2147483648)
		nbr = min_nbr();
	else if (n < 0)
		nbr = neg_nbr(n);
	else
		nbr = pos_nbr(n);
	return (nbr);
}
/*
#include <stdio.h>
int	main(int argc, char **argv)
{
	char	*nbr;
	
	(void)argc;
	nbr = ft_itoa(atoi(argv[1]));
	printf("%s\n", nbr);
	free(nbr);
	return (0);
}
*/
