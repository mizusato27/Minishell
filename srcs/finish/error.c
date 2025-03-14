/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:23:43 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/14 10:34:14 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	perror_prefix(void)
{
	write(STDERR_FILENO, ERROR_PREFIX, ft_strlen(ERROR_PREFIX));
}

void	not_found_cmd(const char *cmd, const char *msg, int status)
{
	perror_prefix();
	write(2, cmd, ft_strlen(cmd));
	write(2, ": ", 2);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(status);
}

void	tokenize_error(const char *location, int *i, char *line)
{
	g_ctx.g_syntax_error = true;
	perror_prefix();
	write(2, "syntax error near ", 18);
	write(2, location, ft_strlen(location));
	write(2, "\n", 1);
	while (line[*i])
		(*i)++;
}

void	parse_error(const char *location, t_token **rest, t_token *tok)
{
	g_ctx.g_syntax_error = true;
	perror_prefix();
	write(2, "syntax error near unexpected token `", 37);
	if (tok->word)
		write(2, tok->word, ft_strlen(tok->word));
	write(2, "' in ", 5);
	write(2, location, ft_strlen(location));
	write(2, "\n", 1);
	while (tok && !at_eof(tok))
		tok = tok->next;
	*rest = tok;
}

void	builtin_error(const char *func, const char *name, const char *err)
{
	perror_prefix();
	if (func)
		write(STDERR_FILENO, func, ft_strlen(func));
	if (name)
	{
		write(STDERR_FILENO, ": ", 2);
		write(STDERR_FILENO, name, ft_strlen(name));
	}
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, err, ft_strlen(err));
	write(STDERR_FILENO, "\n", 1);
}
