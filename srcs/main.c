/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:23:43 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/24 19:39:08 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;                     // 引数のsigは使用しない
	write(1, "\nminishell$ ", 12); // Ctrl-Cで新しいプロンプトを表示
}

void	handle_sigquit(int sig)
{
	(void)sig; // Ctrl-\で何もしない
}

int		last_status = 0;

int	main(void)
{
	char	*line;

	//デバッグのために標準出力にしているが、後々削除
	rl_outstream = stderr;
	setup_signal();
	initenv();
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
		interpret(line, &last_status);
		// TODO: intepret line as a command
		free(line);
	}
	exit(last_status);
}
