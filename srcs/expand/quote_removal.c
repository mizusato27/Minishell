/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 15:38:41 by mizusato          #+#    #+#             */
/*   Updated: 2025/03/09 19:31:02 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_removal(char **dst, char **rest, char *ptr)
{
	char	quote_flag;

	quote_flag = *ptr;
	if (quote_flag != SINGLE_QUOTE && quote_flag != DOUBLE_QUOTE)
		assert_error("Expected quote");
	ptr++;
	while (*ptr != quote_flag)
	{
		if (*ptr == '\0')
			assert_error("Unclosed double quote");
		add_char(dst, *ptr++);
	}
	ptr++;
	*rest = ptr;
}

static void	check_space(char **dst, char **rest, char *ptr, char *top)
{
	char	*tmp;

	tmp = ptr;
	if (ft_isspace(*ptr))
	{
		while (ft_isspace(*ptr))
			ptr++;
		*rest = ptr;
		if (*ptr == '\0')
			return ;
		if (tmp != top)
			add_char(dst, ' ');
	}
	else
	{
		add_char(dst, *tmp++);
		*rest = tmp;
	}
}

static void	remove_quote(t_token *token)
{
	char	*new_word;
	char	*word;
	char	*top;

	if (token == NULL || token->kind != TK_WORD || token->word == NULL)
		return ;
	word = token->word;
	top = word;
	new_word = ft_calloc(1, sizeof(char));
	if (new_word == NULL)
		malloc_error(ER_CALLOC);
	while (*word)
	{
		if (*word == SINGLE_QUOTE || *word == DOUBLE_QUOTE)
			process_removal(&new_word, &word, word);
		else
			check_space(&new_word, &word, word, top);
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
	expand_quote_removal(node->command);
	expand_quote_removal(node->next);
}
