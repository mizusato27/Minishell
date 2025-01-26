/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 13:16:51 by mizusato          #+#    #+#             */
/*   Updated: 2024/04/22 13:14:53 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	i;

	if (!dst && !src)
		return (NULL);
	if (dst > src)
	{
		while (len--)
			((unsigned char *)dst)[len] = ((unsigned char *)src)[len];
	}
	else
	{
		i = 0;
		while (i < len)
		{
			((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
			i++;
		}
	}
	return (dst);
}
/*
#include <stdio.h>
#include <string.h>

int main() {
    char str[] = "Hello, world!";
    char buffer[20];

    // メモリ領域の重なりがある場合
    // memcpyを使用した場合
    memcpy(buffer, str, strlen(str) + 1);
    printf("memcpyの結果 (重なりあり): %s\n", buffer);

    // memmoveを使用した場合
    memmove(str + 6, str, strlen(str) - 5);
    printf("memmoveの結果 (重なりあり): %s\n", str);

    return 0;
}
*/
