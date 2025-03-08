/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_arg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:17:38 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/08 18:26:23 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	*ft_realloc(void *ptr, size_t new_size)
{
	void	*new_ptr;

	if (ptr == NULL)
	{
		new_ptr = malloc(new_size);
		if (new_ptr == NULL)
			malloc_error(ER_MALLOC);
		return (new_ptr);
	}
	new_ptr = malloc(new_size);
	if (new_ptr == NULL)
		malloc_error(ER_MALLOC);
	ft_memcpy(new_ptr, ptr, new_size);
	free(ptr);
	return (new_ptr);
}

// 新しいサイズでメモリを再確保
static char	**tail_recursive(t_token *tok, int nargs, char **argv)
{
	char	**new_argv;

	if (tok == NULL || tok->kind == TK_EOF)
		return (argv);
	new_argv = ft_realloc(argv, (nargs + 2) * sizeof(char *));
	if (new_argv == NULL)
	{
		free(argv);
		malloc_error(ER_REALLOC);
	}
	argv = new_argv;
	argv[nargs] = ft_strdup(tok->word);
	if (argv[nargs] == NULL)
	{
		free(argv);
		malloc_error(ER_STRDUP);
	}
	argv[nargs + 1] = NULL;
	return (tail_recursive(tok->next, nargs + 1, argv));
}

char	**token_list_to_argv(t_token *tok)
{
	char	**argv;

	argv = ft_calloc(1, sizeof(char *));
	if (argv == NULL)
		malloc_error(ER_CALLOC);
	return (tail_recursive(tok, 0, argv));
}
