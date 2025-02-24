/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:23:43 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/25 00:41:29 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	last_status;// <--- special param

int		last_status = 0;

int	main(void)
{
	char	*line;
	// int		status;

	//デバッグのために標準出力にしているが、後々削除
	rl_outstream = stderr;
	setup_signal();
	initenv();
	// status = 0;
	last_status = 0;
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
		interpret(line, &last_status);
		// TODO: intepret line as a command
		free(line);
	}
	// exit(status);
	exit(last_status);
}
