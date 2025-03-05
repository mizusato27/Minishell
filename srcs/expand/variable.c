/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 15:38:57 by mizusato          #+#    #+#             */
/*   Updated: 2025/03/05 17:23:11 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_variable(char *str)
{
	if (str[0] != '$')
		return (0);
	return (is_alpha_under(str[1]));
}

void	expand_var_str(char **dst, char **rest, char *ptr)
{
	char	*name;
	char	*value;

	name = ft_calloc(1, sizeof(char));
	if (name == NULL)
		malloc_error(ER_MALLOC_CALLOC);
	if (*ptr != '$')
		assert_error("Expected dollar sign");
	ptr++;
	if (!is_alpha_under(*ptr))
		assert_error(
			"Variable must starts with alphabetic character or underscore.");
	add_char(&name, *ptr++);
	while (is_alpha_num_under(*ptr))
		add_char(&name, *ptr++);
	value = xgetenv(name);
	free(name);
	if (value)
		while (*value)
			add_char(dst, *value++);
	*rest = ptr;
}

void	add_quote(char **dst, char **rest, char *ptr)
{
	char	type_of_quote;
	int		flag;

	flag = 0;
	type_of_quote = *ptr;
	if (type_of_quote == DOUBLE_QUOTE)
		flag++;
	else if (type_of_quote != SINGLE_QUOTE)
		assert_error("Expected quote");
	add_char(dst, *ptr++);
	while (*ptr != type_of_quote)
	{
		if (*ptr == '\0')
			assert_error("Unclosed double quote");
		else if (flag == 1 && is_variable(ptr))
			expand_var_str(dst, &ptr, ptr);
		else if (flag == 1 && is_special_param(ptr))
			expand_special_param_str(dst, &ptr, ptr);
		else
			add_char(dst, *ptr++);
	}
	add_char(dst, *ptr++);
	*rest = ptr;
}

static void	expand_var_token(t_token *tok)
{
	char	*new_str;
	char	*ptr;

	if (tok == NULL || tok->kind != TK_WORD || tok->word == NULL)
		return ;
	ptr = tok->word;
	new_str = ft_calloc(1, sizeof(char));
	if (!new_str)
		malloc_error(ER_MALLOC_CALLOC);
	while (*ptr && !is_metacharacter(*ptr))
	{
		if (*ptr == SINGLE_QUOTE || *ptr == DOUBLE_QUOTE)
			add_quote(&new_str, &ptr, ptr);
		else if (is_variable(ptr))
			expand_var_str(&new_str, &ptr, ptr);
		else if (is_special_param(ptr))
			expand_special_param_str(&new_str, &ptr, ptr);
		else
			add_char(&new_str, *ptr++);
	}
	free(tok->word);
	tok->word = new_str;
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
