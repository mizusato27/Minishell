/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 15:38:57 by mizusato          #+#    #+#             */
/*   Updated: 2025/03/15 17:43:35 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_variable(char *str)
{
	if (str[0] != '$')
		return (0);
	return (is_alpha_under(str[1]));
}

void	expand_var_str(t_map *envmap, char **dst, char **rest, char *ptr)
{
	char	*name;
	char	*value;

	name = ft_calloc(1, sizeof(char));
	if (name == NULL)
		malloc_error(ER_CALLOC);
	if (*ptr != '$')
		assert_error("Expected dollar sign");
	ptr++;
	if (!is_alpha_under(*ptr))
		assert_error(ERROR_BAD_CHAR);
	add_char(&name, *ptr++);
	while (is_alpha_num_under(*ptr))
		add_char(&name, *ptr++);
	value = xgetenv(envmap, name);
	free(name);
	if (value)
		while (*value)
			add_char(dst, *value++);
	*rest = ptr;
}

void	add_quote(t_map *envmap, char **dst, char **rest,
		int *status)
{
	char	type_of_quote;
	int		flag;
	char	*ptr;

	ptr = *rest;
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
			expand_var_str(envmap, dst, &ptr, ptr);
		else if (flag == 1 && is_special_param(ptr))
			expand_special_param_str(dst, &ptr, ptr, status);
		else
			add_char(dst, *ptr++);
	}
	add_char(dst, *ptr++);
	*rest = ptr;
}

static void	expand_var_token(t_map *envmap, t_token *tok, int *status)
{
	char	*new_str;
	char	*ptr;

	if (tok == NULL || tok->kind != TK_WORD || tok->word == NULL)
		return ;
	ptr = tok->word;
	new_str = ft_calloc(1, sizeof(char));
	if (!new_str)
		malloc_error(ER_CALLOC);
	while (*ptr && !is_metacharacter(*ptr))
	{
		if (*ptr == SINGLE_QUOTE || *ptr == DOUBLE_QUOTE)
			add_quote(envmap, &new_str, &ptr, status);
		else if (is_variable(ptr))
			expand_var_str(envmap, &new_str, &ptr, ptr);
		else if (is_special_param(ptr))
			expand_special_param_str(&new_str, &ptr, ptr, status);
		else
			add_char(&new_str, *ptr++);
	}
	free(tok->word);
	tok->word = new_str;
	expand_var_token(envmap, tok->next, status);
}

void	expand_variable(t_map *envmap, t_node *node, int *status)
{
	if (node == NULL)
		return ;
	expand_var_token(envmap, node->args, status);
	expand_var_token(envmap, node->filename, status);
	expand_variable(envmap, node->redirects, status);
	expand_variable(envmap, node->command, status);
	expand_variable(envmap, node->next, status);
}
