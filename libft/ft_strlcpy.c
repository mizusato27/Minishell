/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 09:42:40 by mizusato          #+#    #+#             */
/*   Updated: 2024/04/22 13:41:19 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t dstsize)
{
	size_t	i;

	if (dstsize == 0)
		return (ft_strlen(src));
	i = 0;
	while (i < (dstsize - 1) && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (ft_strlen(src));
}
/*
#include <stdio.h>
#include <string.h>
int	main(int argc, char **argv)
{
//	char	dest1[256];
	char	dest2[256];
	char	*test1 = NULL;

	(void)argc;
//	printf("%lu\n%s\n", strlcpy(dest1, test1, atoi(argv[2])), dest1);
	printf("%zu\n%s\n", ft_strlcpy(dest2, test1, atoi(argv[2])), dest2);
	return (0);
}
*/
