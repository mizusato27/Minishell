#include "minishell.h"

int	ft_pipe(int pipefd[2])
{
	if (pipe(pipefd) < 0)
		fatal_error(ER_PIPE);
	return (0);
}
