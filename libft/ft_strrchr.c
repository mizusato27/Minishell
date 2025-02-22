/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:49:55 by mizusato          #+#    #+#             */
/*   Updated: 2024/04/19 15:21:25 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*end_s;

	if (!c)
	{
		end_s = (char *)s + ft_strlen(s);
		return (end_s);
	}
	end_s = (char *)s + (ft_strlen(s) - 1);
	while ((s <= end_s) && (*end_s != (char)c))
		end_s--;
	if (s <= end_s)
		return (end_s);
	return (NULL);
}
/*
#include <stdio.h>
#include <string.h>
int	main(int argc, char **argv)
{
	(void)argc;
	printf("%s\n", strrchr(argv[1], (int)argv[2][0]));
	printf("%s\n", ft_strrchr(argv[1], (int)argv[2][0]));
	return (0);
}
*/
