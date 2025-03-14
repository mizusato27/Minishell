/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:23:43 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/14 14:40:22 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool		g_syntax_error;
bool		g_rl_intr;
// int			g_status = 0;
t_map		*g_envmap;

int	main(void)
{
	char	*line;
	int		status;

	// g_status = 0;
	status = 0;
	setup_signal(&status);
	initenv();
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
		// interpret_cmd(line, &g_status);
		interpret_cmd(line, &status);
		free(line);
	}
	exit(status);
}
