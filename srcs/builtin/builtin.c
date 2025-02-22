/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:00:56 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/22 21:05:51 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int		exec_builtin(t_node *node)
// {
// 	int		status;
// 	char	**argv;
// 	do_redirect(node->command->redirects);
// 	argv = token_list_to_argv(node->command->args);
// 	if (strcmp(argv[0], "exit") == 0)
// 		status = builtin_exit(argv);
// 	else
// 		todo("exec_builtin");
// 	free_argv(argv);
// 	reset_redirect(node->command->redirects);
// 	return (status);
// }
// bool	is_builtin(t_node *node)
// {
// 	const char		*cmd_name;
// 	char			*builtin_commands[] = {"exit"};
// 	unsigned int	i;
// 	if (node == NULL || node->command == NULL | node->command->args == NULL ||
// 			node->command->args->word == NULL)
// 		return (false);
// 	cmd_name = node->command->args->word;
// 	i = 0;
// 	while (i < sizeof(builtin_commands) / sizeof(*builtin_commands))
// 	{
// 		if (ft_strcmp(cmd_name, builtin_commands[i]) == 0)
// 			return (true);
// 		i++;
// 	}
// 	return (false);
// }
