/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 08:30:28 by mizusato          #+#    #+#             */
/*   Updated: 2024/04/22 14:05:03 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t dstsize)
{
	size_t	len_dest;
	size_t	len_src;
	size_t	max;
	size_t	i;

	len_src = ft_strlen(src);
	if (!dest && dstsize == 0)
		return (len_src);
	len_dest = ft_strlen(dest);
	if (dstsize <= len_dest)
		return (dstsize + len_src);
	max = dstsize - len_dest - 1;
	i = 0;
	while (src[i] && (i < max))
	{
		dest[len_dest + i] = src[i];
		i++;
	}
	dest[len_dest + i] = '\0';
	return (len_dest + len_src);
}
/*
#include <stdio.h>
#include <string.h>
int	main(int argc, char **argv)
{
//	char	dest1[256];
//	char	dest2[256];
	char	*null = NULL;
	(void)argc;
//	strcpy(dest1, argv[1]);
//	strcpy(dest2, argv[1]);
	printf("%lu\n", strlcat(null, argv[2], atoi(argv[3])));
	printf("%zu\n", ft_strlcat(null, argv[2], atoi(argv[3])));
	return (0);
}
*/
