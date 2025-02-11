/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:23:43 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/11 21:34:47 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	syntax_error = false;

// //コマンドがなかったときに、エラーメッセージを表示して終了
void	err_exit(const char *cmd, const char *msg, int status)
{
    write(2, "minishell: ", 11);
    write(2, cmd, strlen(cmd));
    write(2, ": ", 2);
    write(2, msg, ft_strlen(msg));
    write(2, "\n", 1);
    exit(status);
}

//構文エラーがある場合に単語の最後までスキップ
void	tokenize_error(const char *location, int *i, char *line)
{
	syntax_error = true;
	write(2, "minishell: ", 11);
	write(2, "syntax error near ", 18);
	write(2, location, ft_strlen(location));
	write(2, "\n", 1);
	// perror_prefix();
	// dprintf(STDERR_FILENO, "minishell: syntax error near %s\n", location);
	// printf("i:%d\n",*i);
	while (line[*i])
		(*i)++;
}

void	parse_error(const char *location, t_token **rest, t_token *tok)
{
	syntax_error = true;
	// perror_prefix();
	write(2, "minishell: ", 11);
	write(2, "syntax error near unexpected token `", 37);
	write(2, tok->word, ft_strlen(tok->word));
	write(2, "' in ", 5);
	write(2, location, ft_strlen(location));
	write(2, "\n", 1);
	while (tok && !at_eof(tok))
		tok = tok->next;
	*rest = tok;
}

void	error(char *msg)
{
	printf("%s\n", msg);
	exit(2);
}



