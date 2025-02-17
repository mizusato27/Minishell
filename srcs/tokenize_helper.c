/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 00:32:20 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/17 20:19:37 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//空白文字化のチェック
bool	is_blank(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

//メタ文字のチェック
bool	is_metacharacter(char c)
{
	if (is_blank(c))
		return (true);
	return (c && ft_strchr("|&;()<> \t\n", c));
}

//どの制御演算子かのチェック
bool	which_op(const char *s, const char *op)
{
	return (ft_memcmp(s, op, ft_strlen(op)) == 0);
}

//制御演算子かどうかのチェック
bool	is_operator(char c)
{
	char	*operators;
	int		i;

	i = 0;
	operators = OPERATORS;
	while (operators[i])
	{
		if (operators[i] == c)
			return (true);
		i++;
	}
	return (false);
}

//制御演算子の生成
void	init_operators(char *operators[14])
{
	operators[0] = "||";
	operators[1] = "&";
	operators[2] = "&&";
	operators[3] = ";";
	operators[4] = ";;";
	operators[5] = "(";
	operators[6] = ")";
	operators[7] = "|";
	operators[8] = "\n";
	operators[9] = "<<";
	operators[10] = ">>";
	operators[11] = "<";
	operators[12] = ">";
	operators[13] = NULL;
}

//とりあえず置いておく
bool	is_redirection_operator(const char *s)
{
	static char	*const operators[] = {">", "<", ">>", "<<"};
	size_t				i = 0;				
	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (startswith(s, operators[i]))
			return (true);
		i++;
	}
	return (false);
}