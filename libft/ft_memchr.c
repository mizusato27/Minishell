/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:26:22 by mizusato          #+#    #+#             */
/*   Updated: 2024/04/19 16:01:37 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*str;
	size_t				i;

	str = (const unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (str[i] == (unsigned char)c)
			return ((void *)(str + i));
		i++;
	}
	return (NULL);
}
/*
#include <stdio.h>
#include <string.h>
int main() 
{
    char str[] = "Hello, World!";
    char *result1 = ft_memchr(str, '\0', 100);
    if (result1) 
	{
        printf("文字 'a' は %ld バイト目にあります\n", result1 - str + 1);
    }
	else
	{
        printf("文字 'a' は見つかりませんでした\n");
    }
 	char *result2 = memchr(str, '\0', 100);
    if (result2)
	{
        printf("文字 'a' は %ld バイト目にあります\n", result2 - str + 1);
    }
	else
	{
        printf("文字 'a' は見つかりませんでした\n");
    }
    return (0);
}
*/
