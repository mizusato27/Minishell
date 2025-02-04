/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 17:21:42 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/04 21:40:08 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//新しいトークンを作成
//callocを使用することにより、メンバも全て初期化
t_token	*new_token(char *word, t_token_kind kind)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(*token));
	if (token == NULL)
		error(ER_MALLOC_CALLOC);
	token->word = word;
	token->kind = kind;
	return (token);
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

//制御演算子の全て
static void	init_operators(char *operators[10])
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

t_token	*operator(int *i, char *line)
{
	size_t	j;
	char	*op;
	char	*operators[14];

	init_operators(operators);
	j = 0;
	//制御演算子の数だけチェック
	while (j < sizeof(operators) / sizeof(*operators))
	{
		//どの制御演算子かチェック
		if (which_op(line, operators[j]))
		{
			op = ft_strdup(operators[j]);
			if (op == NULL)
				error(ER_MALLOC_STRDUP);
			//制御演算子（op）の長さだけ元のiを進める
			*i += ft_strlen(op);
			return (new_token(op, TK_OP));
		}
		j++;
	}
	// assert_error("Unexpected operator");
	return (NULL);
}

t_token	*word(int *i, char *line)
{
	char	*word;
	char	quote_flag;
	int		j;

	j = 0;
	//メタ文字（tokenの終わりを示す文字）まで続ける
	while (line[j] && !is_metacharacter(line[j]))
	{
		if (line[j] == SINGLE_QUOTE || line[j] == DOUBLE_QUOTE)
		{
			quote_flag = line[j];
			// skip quote
			j++;
			while (line[j] && line[j] != quote_flag)
				j++;
			if (line[j] == '\0')
			{
				tokenize_error("Unclosed quote", &j, line);
				break ;
			}
			// skip quote
			else
				j++;
		}
		else
			j++;
	}
	word = malloc(j + 1);
	if (word == NULL)
		error(ER_MALLOC);
	ft_strncpy(word, line, j);
	*i += ft_strlen(word);
	return (new_token(word, TK_WORD));
}

t_token	*tokenize(char *arg)
{
	t_token	head;
	t_token	*token;
	int		i;

	i = 0;
	syntax_error = false;
	head.next = NULL;
	token = &head;
	while (arg[i])
	{
		while (arg[i] && is_blank(arg[i]))
			i++;
		if (!arg[i])
			break;
		else if (is_operator(arg[i]))
			token->next = operator(&i, arg + i);
		else if (!is_metacharacter(arg[i]))
			token->next = word(&i, arg + i);
		else
			tokenize_error("Unexpected Token", &i, arg);
		token = token->next;
	}
	token->next = new_token(NULL, TK_EOF);
	return (head.next);
}

// int	main(void)
// {
// 	char *s = "   ls -a || grep a;";
// 	t_token *token = tokenize(s);
// 	t_token *tmp;

// 	while (token)
// 	{
// 		printf("word:%s,type:%d\n", token->word, token->kind);
// 		free(token->word);
// 		tmp = token;
// 		token = token->next;
// 		free(tmp);
// 	}
// }