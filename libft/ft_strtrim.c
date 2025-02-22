/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:31:19 by mizusato          #+#    #+#             */
/*   Updated: 2024/04/27 15:33:20 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_set(char const c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static int	find_start_char(char const *s1, char const *set)
{
	int	i;

	i = 0;
	while (s1[i] && is_set(s1[i], set))
		i++;
	return (i);
}

static int	find_end_char(char const *s1, char const *set)
{
	int	len;
	int	i;

	len = 0;
	while (s1[len])
		len++;
	i = 0;
	while ((i < len) && is_set(s1[len - i - 1], set))
		i++;
	return (len - i - 1);
}

static char	*copy_str(char const *s1, char *s2, int start, int end)
{
	int	i;

	i = 0;
	while (start <= end)
	{
		s2[i] = s1[start];
		i++;
		start++;
	}
	s2[i] = '\0';
	return (s2);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*s2;
	int		start;
	int		end;

	if (!s1)
		return (NULL);
	if (!set)
		return ((char *)s1);
	start = find_start_char(s1, set);
	end = find_end_char(s1, set);
	if (start > end)
	{
		s2 = (char *)malloc(1 * sizeof(char));
		s2[0] = '\0';
		return (s2);
	}
	s2 = (char *)malloc((end - start + 2) * sizeof(char));
	if (!s2)
		return (NULL);
	s2 = copy_str(s1, s2, start, end);
	return (s2);
}
/*
#include <stdio.h>
int	main(int argc, char **argv)
{
	char	*s;

	(void)argc;
	s = ft_strtrim(argv[1], argv[2]);
	printf("%s\n", s);
	free(s);
	return (0);
}
*/
