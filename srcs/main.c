/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:23:43 by ynihei            #+#    #+#             */
/*   Updated: 2025/01/26 17:24:35 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*line;

	//デバッグのために標準出力にしているが、後々削除
	rl_outstream = stderr;
	while (1)
	{
        //標準入力で受け取る
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
        //標準入力した文字列の履歴を残す
		if (*line)
			add_history(line);
		interpret(line);
		// TODO: intepret line as a command
		free(line);
	}
	exit(0);
}