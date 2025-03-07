/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:23:43 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/07 23:20:34 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_context	g_ctx = {};

int	main(void)
{
	char	*line;
	// int		status;

	//デバッグのために標準出力にしているが、後々削除
	rl_outstream = stderr;
	setup_signal();
	initenv();
	g_ctx.g_status = 0;
	while (1)
	{
		//標準入力で受け取る
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		//標準入力した文字列の履歴を残す
		if (*line)
			add_history(line);
		// interpret(line, &status);
		interpret_cmd(line, &g_ctx.g_status);
		// TODO: intepret line as a command
		free(line);
	}
	// exit(status);
	exit(g_ctx.g_status);
}
