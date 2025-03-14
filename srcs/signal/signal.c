/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:04:56 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/14 10:35:34 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_handler(int signum)
{
	g_ctx.g_sig = signum;
}

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

void	setup_signal(void)
{
	rl_outstream = stderr;
	if (isatty(STDIN_FILENO))
		rl_event_hook = reset_prompt;
	setup_signal_handlers(SIGQUIT, SIG_IGN);
	setup_signal_handlers(SIGINT, exec_handler);
}

void	reset_signal(void)
{
	setup_signal_handlers(SIGQUIT, SIG_DFL);
	setup_signal_handlers(SIGINT, SIG_DFL);
}
