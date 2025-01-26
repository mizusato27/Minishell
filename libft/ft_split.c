/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:45:34 by mizusato          #+#    #+#             */
/*   Updated: 2024/05/01 18:38:20 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] && s[i] != c)
		{
			while (s[i] && s[i] != c)
				i++;
			count++;
		}
		else
			i++;
	}
	return (count);
}

static int	calculate_wordlen(char const *s, char c)
{
	int	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

static char	*alloc_word(char const *s, char c)
{
	char	*str;
	int		len;
	int		result_cpy;

	len = calculate_wordlen(s, c);
	str = (char *)malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	result_cpy = ft_strlcpy(str, s, len + 1);
	return (str);
}

static char	**split_str(char **word_array, char const *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] && s[i] != c)
		{
			word_array[j] = alloc_word(s + i, c);
			if (!word_array[j])
			{
				while (j >= 0)
					free(word_array[--j]);
				return (NULL);
			}
			j++;
			i += calculate_wordlen(s + i, c);
		}
		else
			i++;
	}
	word_array[j] = NULL;
	return (word_array);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		nb_words;

	if (!s)
		return (NULL);
	nb_words = count_words(s, c);
	result = (char **)malloc((nb_words + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	result = split_str(result, s, c);
	if (!result)
	{
		free(result);
		return (NULL);
	}
	return (result);
}
