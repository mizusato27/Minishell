/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:23:43 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/14 15:44:11 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool		g_syntax_error;
// bool		g_rl_intr;
int			g_status = 0;
t_map		*g_envmap;

int	main(void)
{
	char	*line;
	int		rl_flag;

	g_status = 0;
	rl_flag = setup_signal();
	initenv();
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
		interpret_cmd(line, &g_status, rl_flag);
		free(line);
	}
	exit(g_status);
}
