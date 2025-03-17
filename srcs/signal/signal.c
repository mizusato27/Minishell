/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:04:56 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/17 13:51:50 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig;

void	exec_handler(int signum)
{
	g_sig = signum;
}

static void	setup_signal_handlers(int signum, void (*handler)(int))
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	if (sigaction(signum, &sa, NULL) < 0)
		fatal_error("sigaction");
}

int	reset_prompt(void)
{
	if (g_sig == 0)
		return (0);
	else if (g_sig == SIGINT)
	{
		rl_replace_line("", 0);
		rl_done = 1;
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
