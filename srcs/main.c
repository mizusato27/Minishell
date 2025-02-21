/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:23:43 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/21 09:11:32 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_sigint(int sig)
{
    (void)sig; // 引数のsigは使用しない
    write(1, "\nminishell$ ", 12); // Ctrl-Cで新しいプロンプトを表示
}

void handle_sigquit(int sig)
{
    (void)sig; // Ctrl-\で何もしない
}

int	main(void)
{
	char	*line;
	int		status;

	// Ctrl-Cでプロンプトを新しい行に表示
    signal(SIGINT, handle_sigint);
    // Ctrl-\で何もしない（デフォルト動作を無効にする）
    signal(SIGQUIT, handle_sigquit);
    // Ctrl-Dでシェルを終了する
    signal(SIGTSTP, SIG_IGN); // SIGTSTPを無視して、バックグラウンドにしない	
	//デバッグのために標準出力にしているが、後々削除
	rl_outstream = stderr;
	setup_signal(); //<-- signal.c
	initenv(); //<-- env.c
	status = 0;
	while (1)
	{
        //標準入力で受け取る
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
        //標準入力した文字列の履歴を残す
		if (*line)
			add_history(line);
		interpret(line, &status);
		// TODO: intepret line as a command
		free(line);
	}
	exit(status);
}

// int main(void)
// {
// 	t_map *env = map_new();
	
// 	// 環境変数をセット
// 	map_set(env, "USER", "root");
// 	map_set(env, "HOME", "/root");
// 	map_set(env, "SHELL", "/bin/bash");

// 	// 取得して表示
// 	printf("USER: %s\n", map_get(env, "USER"));
// 	printf("HOME: %s\n", map_get(env, "HOME"));
// 	printf("SHELL: %s\n", map_get(env, "SHELL"));

// 	// 存在しないキー
// 	printf("FOO: %s\n", map_get(env, "FOO"));

// 	// キーを削除
// 	map_unset(env, "USER");
// 	printf("After unset USER: %s\n\n", map_get(env, "USER"));

// 	map_set(env, "HOME", "/root/home");
// 	map_set(env, "TEST", NULL);
	
// 	t_item	*cur;
	
// 	cur = env->item_head.next;
// 	while (cur)
// 	{
// 		printf("name: %s, value: %s\n", cur->name, cur->value);
// 		cur = cur->next;
// 	}

// 	// メモリリーク防止のための解放処理（map_free関数を実装するべき）
// 	// map_free(env);

// 	return 0;
// }