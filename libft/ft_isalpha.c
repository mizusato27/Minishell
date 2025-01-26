/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 10:21:21 by mizusato          #+#    #+#             */
/*   Updated: 2024/04/18 10:41:47 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}
/*
#include <stdio.h>
#include <ctype.h>
int	main(int argc, char **argv)
{
	int	c;

	(void)argc;
	c = argv[1][0];
	printf("%d\n", isalpha(c));
	printf("%d\n", ft_isalpha(c));
	return (0);
}
*/
