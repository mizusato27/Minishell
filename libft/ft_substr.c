/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 09:13:21 by mizusato          #+#    #+#             */
/*   Updated: 2024/04/27 15:33:32 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*alloc(char const *s, char *sub, unsigned int start, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		sub[i] = s[start];
		i++;
		start++;
	}
	sub[i] = '\0';
	return (sub);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub_str;
	size_t	count_len;

	if (!s)
		return (NULL);
	count_len = ft_strlen(s);
	if (start >= count_len)
	{
		sub_str = (char *)malloc(1 * sizeof(char));
		if (sub_str == NULL)
			return (NULL);
		sub_str[0] = '\0';
		return (sub_str);
	}
	if (len > (count_len - start))
		len = count_len - start;
	sub_str = (char *)malloc((len + 1) * sizeof(char));
	if (sub_str == NULL)
		return (NULL);
	sub_str = alloc(s, sub_str, start, len);
	return (sub_str);
}
/*
#include <stdio.h>
int	main(int argc, char **argv)
{
	char	*str;

	(void)argc;
	str = ft_substr(argv[1], atoi(argv[2]), atoi(argv[3]));
	printf("%s\n", str);
	free(str);
	return (0);
}
*/
