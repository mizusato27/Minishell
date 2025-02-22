/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 00:28:56 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/22 17:32:21 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void	todo(char *message)
{
	printf("TODO: %s\n", message);
}

//文字列の最後に文字を追加
void	append_char(char **s, char c)
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
void	remove_quote(t_token *token)
{
	char	*new_word;
	char	*word;
	char	quote_flag;

	if (token == NULL || token->kind != TK_WORD || token->word == NULL)
		return ;
	word = token->word;
	new_word = NULL;
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
				append_char(&new_word, *word++);
			}
			word++;
		}
		else
			append_char(&new_word, *word++);
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
	expand_quote_removal(node->next);
}

void	expand(t_node *node)
{
	expand_quote_removal(node);
}

// int main()
// {
	
// 	return (0);
// }