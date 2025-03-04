/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 15:39:16 by mizusato          #+#    #+#             */
/*   Updated: 2025/03/04 23:21:31 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_new_pipe(t_node *node)
{
	if (node->next == NULL)
		return ;
	ft_pipe(node->outpipe);
	node->next->inpipe[0] = node->outpipe[0];
	node->next->inpipe[1] = node->outpipe[1];
}

void	process_child_pipe(t_node *node)
{
	ft_close(node->outpipe[0]);
	ft_dup2(node->inpipe[0], STDIN_FILENO);
	if (node->inpipe[0] != STDIN_FILENO)
		ft_close(node->inpipe[0]);
	ft_dup2(node->outpipe[1], STDOUT_FILENO);
	if (node->outpipe[1] != STDOUT_FILENO)
		ft_close(node->outpipe[1]);
}

void	process_parent_pipe(t_node *node)
{
	if (node->inpipe[0] != STDIN_FILENO)
		ft_close(node->inpipe[0]);
	if (node->next)
		ft_close(node->outpipe[1]);
}
