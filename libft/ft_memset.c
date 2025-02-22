/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 12:07:18 by mizusato          #+#    #+#             */
/*   Updated: 2024/04/19 12:50:12 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			i;

	i = 0;
	while (i < len)
	{
		((unsigned char *)b)[i] = (unsigned char)c;
		i++;
	}
	return (b);
}
/*
#include <stdio.h>
#include <string.h>
int main()
{
    char str1[20];
    char str2[20];
	int arr[8];
    struct Data {
        char a;
        int b;
        double c;
    } data;

    // 文字列の初期化
    printf("Before ft_memset: %s\n", str1);
    ft_memset(str1, 'x', sizeof(str1));
    printf("After ft_memset: %.*s\n", (int)sizeof(str1), str1);
	printf("Before memset: %s\n", str2);
    memset(str2, 'x', sizeof(str2));
    printf("After memset: %.*s\n\n", (int)sizeof(str2), str2);

    // 整数配列の初期化
    printf("Before ft_memset: ");
    for (int i = 0; i < 8; i++)
        printf("%d ", arr[i]);
    printf("\n");

    ft_memset(arr, 42, sizeof(arr));

    printf("After ft_memset: ");
    for (int i = 0; i < 8; i++)
        printf("%d ", arr[i]);
    printf("\n\n");

    // 構造体の初期化
    printf("Before ft_memset:\n");
    printf("a = %d, b = %d, c = %f\n", data.a, data.b, data.c);

    ft_memset(&data, 0, sizeof(data));

    printf("After ft_memset:\n");
    printf("a = %d, b = %d, c = %f\n", data.a, data.b, data.c);

    return 0;
}
*/
