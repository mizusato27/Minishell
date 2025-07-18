/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:04:56 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/10 16:02:33 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// シグナルハンドラ関数
// 受信したシグナル番号をグローバル変数 'sig' に保存する
void	exec_handler(int signum)
{
	g_ctx.g_sig = signum;
}

//指定したシグナルを設定する（無視するか、指定されたハンドラを設定する）
// SIG_IGNはシグナルを無視
static void	setup_signal_handlers(int signum, void (*handler)(int))
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (handler == SIG_IGN)
		sa.sa_handler = SIG_IGN;
	else if (handler == SIG_DFL)
		sa.sa_handler = SIG_DFL;
	else
		sa.sa_handler = handler;
	if (sigaction(signum, &sa, NULL) < 0)
		fatal_error("sigaction");
}

// シグナルの状態をチェックし、必要な処理を行う
// SIG_INTは Ctr+C
// rl_replace_line("", 0); // 入力行をクリア
// rl_done = 1;            // readline ループを終了
// g_ctx.g_rl_intr = true; 割り込み操作
// rl_event_hookの関係により、status 0 を返す
int	reset_prompt(void)
{
	if (g_ctx.g_sig == 0)
		return (0);
	else if (g_ctx.g_sig == SIGINT)
	{
		g_ctx.g_sig = 0;
		g_ctx.g_rl_intr = true;
		rl_replace_line("", 0);
		rl_done = 1;
		g_ctx.g_status = 130;
		return (0);
	}
	return (0);
}

// 必要なシグナル設定を行う
// SIGQUIT を無視し、SIGINT を適切に処理する
// readline の出力先を標準エラー出力に設定
// isattyはファイルディスクリプタが端末かどうかを判定する
void	setup_signal(void)
{
	rl_outstream = stderr;
	if (isatty(STDIN_FILENO))
		rl_event_hook = reset_prompt;
	setup_signal_handlers(SIGQUIT, SIG_IGN);
	setup_signal_handlers(SIGINT, exec_handler);
}

//指定されたシグナルをデフォルトの動作にリセットする
// SIGQUIT および SIGINT のシグナル動作をリセット
void	reset_signal(void)
{
	setup_signal_handlers(SIGQUIT, SIG_DFL);
	setup_signal_handlers(SIGINT, SIG_DFL);
}
