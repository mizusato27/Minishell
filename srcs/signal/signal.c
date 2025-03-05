/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:04:56 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/05 18:43:09 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// シグナルハンドラ関数
// 受信したシグナル番号をグローバル変数 'sig' に保存する
void	handler(int signum)
{
	g_ctx.g_sig = signum;
}

/**
 * 指定したシグナルを無視する設定を行う
 */
void	ignore_sig(int signum)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN; // シグナルを無視する
	if (sigaction(signum, &sa, NULL) < 0)
		fatal_error("sigaction");
}

/**
 * SIGINT（Ctrl+C）のシグナルハンドラを設定する
 * シグナルを受信した際に handler 関数が呼ばれる
 */
void	setup_sigint(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler; // SIGINT を受けた際に handler を呼び出す
	if (sigaction(SIGINT, &sa, NULL) < 0)
		fatal_error("sigaction");
}

/**
 * シグナルの状態をチェックし、必要な処理を行う
 * @return 処理結果（常に 0 を返す）
 */
int	check_state(void)
{
	if (g_ctx.g_sig == 0)
		return (0);
	else if (g_ctx.g_sig == SIGINT) // Ctrl+C が押された場合
	{
		g_ctx.g_sig = 0;
		g_ctx.g_rl_intr = true;
		rl_replace_line("", 0); // 入力行をクリア
		rl_done = 1;            // readline ループを終了
		return (0);
	}
	return (0);
}

/**
 * 必要なシグナル設定を行う
 * SIGQUIT を無視し、SIGINT を適切に処理する
 */
//isattyはファイルディスクリプタが端末かどうかを判定する
void	setup_signal(void)
{
	extern int	_rl_echo_control_chars;

	_rl_echo_control_chars = 0; // Ctrl+文字 の表示を無効化
	rl_outstream = stderr;      // readline の出力先を標準エラー出力に設定
	if (isatty(STDIN_FILENO))
		rl_event_hook = check_state; // シグナルチェックをイベントフックに設定
	ignore_sig(SIGQUIT);             // SIGQUIT を無視する
	setup_sigint();                  // SIGINT の設定
}

/**
 * シグナルの動作をデフォルトにリセットする
 * @param signum 対象のシグナル番号
 */
void	reset_sig(int signum)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL; // デフォルトのシグナル動作
	if (sigaction(signum, &sa, NULL) < 0)
		fatal_error("sigaction");
}

/**
 * 指定されたシグナルをデフォルトの動作にリセットする
 * SIGQUIT および SIGINT のシグナル動作をリセット
 */
//あまりいる理由が分からない
void	reset_signal(void)
{
	reset_sig(SIGQUIT);
	reset_sig(SIGINT);
}
