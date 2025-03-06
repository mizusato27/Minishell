/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_arg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:17:38 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/06 19:19:19 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// reallocは後で変更必須
// 新しいサイズでメモリを再確保
char	**tail_recursive(t_token *tok, int nargs, char **argv)
{
	char	**new_argv;

	if (tok == NULL || tok->kind == TK_EOF)
		return (argv);
	new_argv = realloc(argv, (nargs + 2) * sizeof(char *));
	if (new_argv == NULL)
	{
		free(argv);
		malloc_error("realloc");
	}
	argv = new_argv;
	argv[nargs] = ft_strdup(tok->word);
	if (argv[nargs] == NULL)
	{
		free(argv);
		malloc_error(ER_MALLOC_STRDUP);
	}
	argv[nargs + 1] = NULL;
	return (tail_recursive(tok->next, nargs + 1, argv));
}

char	**token_list_to_argv(t_token *tok)
{
	char	**argv;

	argv = ft_calloc(1, sizeof(char *));
	if (argv == NULL)
		malloc_error(ER_MALLOC_CALLOC);
	return (tail_recursive(tok, 0, argv));
}
