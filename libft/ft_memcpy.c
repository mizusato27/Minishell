/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:56:42 by mizusato          #+#    #+#             */
/*   Updated: 2024/04/22 13:11:05 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	if (!dst && !src)
		return (NULL);
	i = 0;
	while (i < n)
	{
		((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dst);
}
/*
#include <stdio.h>
#include <string.h>
int	main(int argc, char **argv)
{
	char	dest[10];
	int		len;

	(void)argc;
	len = 0;
	while (argv[1][len])
		len++;
	memcpy(dest, argv[1], len);
	dest[len - 1] = '\0';
	printf("%s\n", dest);
	return (0);
}
*/
