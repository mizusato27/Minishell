/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 17:21:42 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/08 18:25:55 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//制御演算子かどうかのチェック
// j < sizeof(operators) / sizeof(*operators)は制御演算子の数だけチェック
t_token	*operator(int *i, char *line)
{
	size_t	j;
	char	*op;
	char	*operators[7];

	init_operators(operators);
	j = 0;
	while (j < sizeof(operators) / sizeof(*operators))
	{
		if (which_op(line, operators[j]))
		{
			op = ft_strdup(operators[j]);
			if (op == NULL)
				malloc_error(ER_STRDUP);
			*i += ft_strlen(op);
			return (new_token(op, TK_OP));
		}
		j++;
	}
	fatal_error("Unexpected operator");
	return (NULL);
}

//文字列をトークンに分割
static	int	parse_word_length(char *line)
{
	char	quote_flag;
	int		j;

	j = 0;
	while (line[j] && !is_metacharacter(line[j]))
	{
		if (line[j] == SINGLE_QUOTE || line[j] == DOUBLE_QUOTE)
		{
			quote_flag = line[j];
			j++;
			while (line[j] && line[j] != quote_flag)
				j++;
			if (line[j] == '\0')
			{
				tokenize_error("Unclosed quote", &j, line);
				break ;
			}
			else
				j++;
		}
		else
			j++;
	}
	return (j);
}

//単語を取得
//メタ文字（tokenの終わりを示す文字）まで続ける
t_token	*word(int *i, char *line)
{
	char	*word;
	int		j;

	j = parse_word_length(line);
	word = malloc(j + 1);
	if (word == NULL)
		malloc_error(ER_MALLOC);
	ft_strncpy(word, line, j);
	*i += ft_strlen(word);
	return (new_token(word, TK_WORD));
}

static	t_token	*init_tokenize(t_token *head, int *i)
{
	*i = 0;
	g_ctx.g_syntax_error = false;
	head->next = NULL;
	return (head);
}

// head.nextに最初のトークンを格納
t_token	*tokenize(char *arg)
{
	t_token	head;
	t_token	*token;
	int		i;

	token = init_tokenize(&head, &i);
	while (arg[i])
	{
		while (arg[i] && is_blank(arg[i]))
			i++;
		if (!arg[i])
			break ;
		else if (is_operator(&arg[i]))
			token->next = operator(&i, arg + i);
		else if (!is_metacharacter(arg[i]) && arg[i])
			token->next = word(&i, arg + i);
		else
		{
			tokenize_error("Unexpected Token", &i, arg);
			return (NULL);
		}
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

// 	tmp = token;
// 	while (tmp)
// 	{
// 		printf("word:%s,type:%d\n", tmp->word, tmp->kind);
// 		tmp = tmp->next;
// 	}
// 	free_token(token);
// }
