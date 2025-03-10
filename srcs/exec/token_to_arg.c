/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_arg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:17:38 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/10 15:33:43 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	copy_size;

	if (ptr == NULL)
		return (malloc(new_size));
	new_ptr = malloc(new_size);
	if (new_ptr == NULL)
		malloc_error(ER_MALLOC);
	if (old_size < new_size)
		copy_size = old_size;
	else
		copy_size = new_size;
	ft_memcpy(new_ptr, ptr, copy_size);
	free(ptr);
	return (new_ptr);
}

static char	**expand_argv(char **argv, int nargs)
{
	char	**new_argv;
	size_t	old_size;
	size_t	new_size;

	old_size = (nargs + 1) * sizeof(char *);
	new_size = (nargs + 2) * sizeof(char *);
	new_argv = ft_realloc(argv, old_size, new_size);
	if (new_argv == NULL)
	{
		free_argv(argv);
		malloc_error(ER_REALLOC);
	}
	return (new_argv);
}

char	**token_list_to_argv(t_token *tok)
{
	int		nargs;
	char	**argv;

	nargs = 0;
	argv = malloc(sizeof(char *));
	if (argv == NULL)
		malloc_error(ER_MALLOC);
	argv[0] = NULL;
	while (tok != NULL && tok->kind != TK_EOF)
	{
		argv = expand_argv(argv, nargs);
		argv[nargs] = ft_strdup(tok->word);
		if (argv[nargs] == NULL)
		{
			free_argv(argv);
			malloc_error(ER_STRDUP);
		}
		nargs++;
		argv[nargs] = NULL;
		tok = tok->next;
	}
	return (argv);
}
