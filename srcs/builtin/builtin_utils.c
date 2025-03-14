/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 23:30:40 by mizusato          #+#    #+#             */
/*   Updated: 2025/03/14 11:18:03 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	chdir_ex(char *path)
{
	if (chdir(path) < 0)
	{
		builtin_error("cd", NULL, "chdir");
		return (-1);
	}
	return (0);
}

int	map_set_value_ex(const char *name, const char *pwd)
{
	if (map_set_value(g_envmap, name, pwd) < 0)
	{
		builtin_error("cd", NULL, "map_set");
		return (-1);
	}
	return (0);
}

int	process_minus_option(char *old_path, char *current_pwd)
{
	char	path[PATH_MAX];

	if (old_path == NULL)
	{
		builtin_error("cd", NULL, "OLDPWD not set");
		return (1);
	}
	if (chdir_ex(old_path) < 0)
		return (1);
	if (getcwd(path, PATH_MAX) == NULL)
	{
		builtin_error("cd", NULL, "getcwd");
		return (1);
	}
	ft_putstr_fd(old_path, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	if (map_set_value_ex("OLDPWD", current_pwd) < 0)
		return (1);
	if (map_set_value_ex("PWD", path) < 0)
		return (1);
	return (0);
}

static int	cpy_dir_path(char *path, char *arg)
{
	char	*home_dir_path;

	if (arg == NULL || ft_strcmp(arg, "~") == 0
		|| ft_strcmp(arg, "--") == 0)
	{
		home_dir_path = map_get_value(g_envmap, "HOME");
		if (home_dir_path == NULL)
		{
			builtin_error("cd", NULL, "HOME not set");
			return (-1);
		}
		ft_strlcpy(path, home_dir_path, PATH_MAX);
	}
	else
		ft_strlcpy(path, arg, PATH_MAX);
	return (0);
}

int	cpy_home_path(char *path, char *arg)
{
	char	*home_dir_path;

	if (!arg)
		return (cpy_dir_path(path, NULL));
	if (arg[0] != '~')
		return (cpy_dir_path(path, arg));
	home_dir_path = map_get_value(g_envmap, "HOME");
	if (home_dir_path == NULL)
	{
		builtin_error("cd", NULL, "HOME not set");
		return (-1);
	}
	ft_strlcpy(path, home_dir_path, PATH_MAX);
	if (arg[1] == '/' && ft_strlen(arg) > 2)
	{
		if (ft_strlcat(path, &arg[1], PATH_MAX) >= PATH_MAX)
			assert_error(ER_OVERFLOW);
	}
	else if (arg[1] != '\0' && arg[1] != '/')
	{
		if (ft_strlcpy(path, arg, PATH_MAX) >= PATH_MAX)
			assert_error(ER_OVERFLOW);
	}
	return (0);
}
