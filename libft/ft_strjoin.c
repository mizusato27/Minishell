/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 10:42:24 by mizusato          #+#    #+#             */
/*   Updated: 2024/05/02 23:13:19 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	join_strs(char const *s1, char const *s2, char *str)
{
	while (*s1)
	{
		*str = *s1;
		str++;
		s1++;
	}
	while (*s2)
	{
		*str = *s2;
		str++;
		s2++;
	}
	*str = '\0';
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	char	*start_str;

	// if (!s1)
	// {
	// 	str = ft_strdup(s2);
	// 	return (str);
	// }
	// if (!s2)
	// {
	// 	str = ft_strdup(s1);
	// 	return (str);
	// }
	if (!s1 || !s2)
		return (NULL);
	str = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	start_str = str;
	join_strs(s1, s2, str);
	return (start_str);
}
/*
#include <stdio.h>
int	main(int argc, char **argv)
{
	char	*str;

	(void)argc;
	str = ft_strjoin(argv[1], argv[2]);
	printf("%s\n", str);
	free(str);
	return (0);
}
*/
