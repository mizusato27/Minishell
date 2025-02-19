/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 19:04:55 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/19 15:38:56 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void	free_node(t_node *node)
{
	if (node == NULL)
		return ;
	free_token(node->args);
	free_token(node->filename);// <--- added
	free_token(node->delimiter);//<--- added
	free_node(node->redirects);// <--- added
	free_node(node->next);
	free_node(node->command);// <--- pipe
	free(node);
}

void	free_token(t_token *tok)
{
	if (tok == NULL)
		return ;
	if (tok->word)
		free(tok->word);
	free_token(tok->next);
	free(tok);
}

void	free_argv(char **argv)
{
	int	i;

	if (argv == NULL)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}
