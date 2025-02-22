/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:15:06 by mizusato          #+#    #+#             */
/*   Updated: 2024/04/27 15:21:15 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

static int	is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	nbr;

	i = 0;
	sign = 1;
	nbr = 0;
	while (str[i] && is_space(str[i]))
		i++;
	if (str[i] && (str[i] == '+' || str[i] == '-'))
	{
		if (str[i] == '-')
			sign *= (-1);
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if ((nbr > (LONG_MAX - (str[i] - '0')) / 10) && sign == 1)
			return ((int)LONG_MAX);
		else if ((nbr * (-1) < (LONG_MIN + (str[i] - '0')) / 10) && sign == -1)
			return ((int)LONG_MIN);
		nbr = nbr * 10 + (str[i] - '0');
		i++;
	}
	return (nbr * sign);
}

//#include <stdio.h>
// int	main(int argc, char **argv)
// {
// 	int	nbr;

// 	(void)argc;
// 	nbr = ft_atoi(argv[1]);
// 	printf("real atoi:%d\n", atoi(argv[1]));
// 	printf("  my atoi:%d\n", nbr);
// 	return (0);
// }

// int	main(void)
// {
// 	printf("\n");
// 	printf("/////////////////////////\n");
// 	printf("\n");

// 	printf("UINT_MAX + 1\n");
// 	printf("real:%d\n", atoi("4294967296"));
// 	printf("  my:%d\n\n", ft_atoi("4294967296"));

// 	printf("LONG_MAX + 1\n");
// 	printf("real:%d\n", atoi("9223372036854775808"));
// 	printf("  my:%d\n\n", ft_atoi("9223372036854775808"));

// 	printf("LONG_MIN - 1\n");
// 	printf("real:%d\n", atoi("-9223372036854775809"));
// 	printf("  my:%d\n\n", ft_atoi("-9223372036854775809"));

// 	printf("ULONG_MAX(SIZE_MAX) + 1\n");
// 	printf("real:%d\n", atoi("18446744073709551616"));
// 	printf("  my:%d\n\n", ft_atoi("18446744073709551616"));

// 	printf("/////////////////////////\n");
// 	printf("\n");

// 	printf("UINT_MAX - 1\n");
// 	printf("real:%d\n", atoi("4294967294"));
// 	printf("  my:%d\n\n", ft_atoi("4294967294"));

// 	printf("LONG_MAX - 1\n");
// 	printf("real:%d\n", atoi("9223372036854775806"));
// 	printf("  my:%d\n\n", ft_atoi("9223372036854775806"));

// 	printf("LONG_MIN + 1 <- miss\n");
// 	printf("real:%d\n", atoi("-9223372036854775807"));
// 	printf("  my:%d\n\n", ft_atoi("-9223372036854775807"));

// 	printf("ULONG_MAX(SIZE_MAX) - 1\n");
// 	printf("real:%d\n", atoi("18446744073709551614"));
// 	printf("  my:%d\n\n", ft_atoi("18446744073709551614"));

// 	printf("/////////////////////////\n");
// 	printf("\n");
// 	return (0);
// }