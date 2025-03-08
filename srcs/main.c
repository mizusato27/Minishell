/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:23:43 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/08 12:42:29 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_context	g_ctx = {};

int	main(void)
{
	char	*line;

	setup_signal();
	initenv();
	g_ctx.g_status = 0;
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
		interpret_cmd(line, &g_ctx.g_status);
		// TODO: intepret line as a command
		free(line);
	}
	exit(g_ctx.g_status);
}
