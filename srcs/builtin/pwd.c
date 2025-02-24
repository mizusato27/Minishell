#include "minishell.h"

static int	is_equal_inode(const char *path1, const char *path2)
{
	struct stat	st1;
	struct stat	st2;

	ft_memset(&st1, 0, sizeof(st1));
	ft_memset(&st2, 0, sizeof(st2));
	if (stat(path1, &st1) < 0)
		fatal_error("stat");
	if (stat(path2, &st2) < 0)
		fatal_error("stat");
	if (st1.st_ino == st2.st_ino)
		return (1);
	return (0);
}

int	builtin_pwd(void)
{
	char	*pwd;
	char	cwd[PATH_MAX];

	pwd = xgetenv("PWD");
	if (pwd == NULL || !is_equal_inode(pwd, "."))
	{
		if (getcwd(cwd, PATH_MAX) == NULL)
		{
			builtin_error("pwd", NULL, "getcwd");
			return (1);
		}
		ft_putstr_fd(cwd, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		return (0);
	}
	else
	{
		ft_putstr_fd(pwd, STDOUT_FILENO);
		ft_putchar_fd('\n', STDERR_FILENO);
		return (0);
	}
}
