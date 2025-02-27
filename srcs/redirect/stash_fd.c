#include "minishell.h"

int	stash_fd(int fd)
{
	int	stash;

	stash = fcntl(fd, F_DUPFD, 10);
	if (stash < 0)
		fatal_error("");
	if (close(fd) < 0)
		fatal_error("");
	return (stash);
}
