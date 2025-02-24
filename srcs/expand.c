/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 00:28:56 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/24 23:26:05 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

//getenvが出てきたら、xgetenvに変更

void	todo(char *message)
{
	printf("TODO: %s\n", message);
}

//文字列の最後に文字を追加
void	add_char(char **s, char c)
{
	size_t	size;
	char	*new;

	size = APPEND_CHAR_SIZE + END_CHAR_SIZE;
	if (*s)
		size += ft_strlen(*s);
	new = malloc(size);
	if (new == NULL)
		error(ER_MALLOC);
	if (*s)
		ft_strlcpy(new, *s, size);
	new[size - 2] = c;
	new[size - 1] = '\0';
	if (*s)
		free(*s);
	*s = new;
}

//クォートを削除
static void	remove_quote(t_token *token)
{
	char	*new_word;
	char	*word;
	char	quote_flag;

	if (token == NULL || token->kind != TK_WORD || token->word == NULL)
		return ;
	word = token->word;
	// new_word = NULL;
	new_word = ft_calloc(1, sizeof(char));// <--- modified
	if (new_word == NULL)
		fatal_error("calloc");
	while (*word && !is_metacharacter(*word))
	{
		if (*word == SINGLE_QUOTE || *word == DOUBLE_QUOTE)
		{
			quote_flag = *word;
			word++;
			while (*word != quote_flag)
			{
				if (*word == '\0')
					todo("Unclosed quote");
				add_char(&new_word, *word++);
			}
			word++;
		}
		else
			add_char(&new_word, *word++);
	}
	free(token->word);
	if (!new_word)
		new_word = ft_strdup("");
	token->word = new_word;
	remove_quote(token->next);
}

void	expand_quote_removal(t_node *node)
{
	if (node == NULL)
		return ;
	remove_quote(node->args);
	remove_quote(node->filename);
	remove_quote(node->delimiter);
	expand_quote_removal(node->redirects);
	expand_quote_removal(node->command);// <--- pipe
	expand_quote_removal(node->next);
}

static int	is_variable(char *str)
{
	if (str[0] != '$')
		return (0);
	return (is_alpha_under(str[1]));
}

static int	is_special_param(char *str)
{
	if (str[0] == '$' && str[1] == '?')
		return (1);
	return (0);
}

static void	add_number(char **dst, unsigned int n)
{
	if (n == 0)
	{
		add_char(dst, '0');
		return ;
	}
	if (n / 10 != 0)
		add_number(dst, n / 10);
	add_char(dst, '0' + (n % 10));
}

static void	expand_special_param_str(char **dst, char **rest, char *ptr)
{
	if (!is_special_param(ptr))
		assert_error("Expected special parameter");
	ptr += 2;
	add_number(dst, last_status);
	*rest = ptr;
}

static void	expand_var_str(char **dst, char **rest, char *ptr)
{
	char	*name;
	char	*value;

	name = ft_calloc(1, sizeof(char));
	if (name == NULL)
		fatal_error("calloc");
	if (*ptr != '$')
		assert_error("Expected dollar sign");
	ptr++;
	if (!is_alpha_under(*ptr))
		assert_error("Variable must starts with alphabetic character or underscore.");
	add_char(&name, *ptr++);
	while (is_alpha_num_under(*ptr))
		add_char(&name, *ptr++);
	value = getenv(name);
	free(name);
	if (value)
		while (*value)
			add_char(dst, *value++);
	*rest = ptr;
}

static void	add_single_quote(char **dst, char **rest, char *ptr)
{
	if (*ptr == SINGLE_QUOTE)
	{
		add_char(dst, *ptr++);
		while (*ptr != SINGLE_QUOTE)
		{
			if (*ptr == '\0')
				assert_error("Unclosed single quote");
			add_char(dst, *ptr++);
		}
		add_char(dst, *ptr++);
		*rest = ptr;
	}
	else
		assert_error("Expected single quote");
}

static void	add_double_quote(char **dst, char **rest, char *ptr)
{
	if (*ptr == DOUBLE_QUOTE)
	{
		add_char(dst, *ptr++);
		while (*ptr != DOUBLE_QUOTE)
		{
			if (*ptr == '\0')
				assert_error("Unclosed double quote");
			else if (is_variable(ptr))
				expand_var_str(dst, &ptr, ptr);
			else if (is_special_param(ptr))
				expand_special_param_str(dst, &ptr, ptr);
			else
				add_char(dst, *ptr++);
		}
		add_char(dst, *ptr++);
		*rest = ptr;
	}
	else
		assert_error("Expected double quote");
}

static void	expand_var_token(t_token *tok)
{
	char	*new_str;// 新しい文字列を格納するためのポインタ
	char	*ptr;

	if (tok == NULL || tok->kind != TK_WORD || tok->word == NULL)
		return ;
	ptr = tok->word;
	new_str = ft_calloc(1, sizeof(char));// 新しい文字列用にメモリを1バイト確保（NUL終端用）
	if (!new_str)
		fatal_error("calloc");
	while (*ptr && !is_metacharacter(*ptr))
	{
		if (*ptr == SINGLE_QUOTE)
			add_single_quote(&new_str, &ptr, ptr);// クォートで囲まれた部分を処理
		else if (*ptr == DOUBLE_QUOTE)
			add_double_quote(&new_str, &ptr, ptr);
		else if (is_variable(ptr))
			expand_var_str(&new_str, &ptr, ptr);// 変数を展開
		else if (is_special_param(ptr))
			expand_special_param_str(&new_str, &ptr, ptr);
		else
			add_char(&new_str, *ptr++);// 通常の文字をそのまま追加
	}
	free(tok->word);
	tok->word = new_str;// 古い文字列を解放し、新しい文字列に置き換え
	expand_var_token(tok->next);
}

void	expand_variable(t_node *node)
{
	if (node == NULL)
		return ;
	expand_var_token(node->args);
	expand_var_token(node->filename);
	expand_variable(node->redirects);
	expand_variable(node->command);
	expand_variable(node->next);
}

void	expand(t_node *node)
{
	expand_variable(node);
	expand_quote_removal(node);
}

// int main()
// {

// 	return (0);
// }
