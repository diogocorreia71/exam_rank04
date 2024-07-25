#include "microshell.h"

int ft_perror(char *str)
{
	while (*str)
		write(2, str++, 1);
	return (1);
}

int ft_cd(char **argv, int i)
{
	if (i != 2)
		return (ft_perror(BAD));
	(void)argv;
	if (chdir(argv[1]) == -1)
		return (ft_perror(PATH), ft_perror(argv[1]), ft_perror("\n"));
	return (0);
}

int ft_exec(char **argv, char **env, int i)
{
	int pid;
	int fd[2];
	int g_exit;
	int has_pipe = argv[i] && !strcmp(argv[i], "|");

	if (has_pipe && pipe(fd) == -1)
		return (ft_perror(FATAL));
	pid = fork();
	if (pid == 0)
	{
		argv[i] = NULL;
		if (has_pipe && (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
			return (ft_perror(FATAL));
		execve(*argv, argv, env);
		return (ft_perror(EXEC), ft_perror(*argv), ft_perror("\n"));
	}
	waitpid(pid, &g_exit, 0);
	if (has_pipe && (dup2(fd[0], 0) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
		return (ft_perror(FATAL));
	return (WEXITSTATUS(g_exit));
}

int main(int argc, char **argv, char **env)
{
	int i = 0;
	int g_exit;

	if (argc < 2)
		return (1);
	while (argv[i] && argv[++i])
	{
		argv += i;
		i = 0;
		while (argv[i] && strcmp(argv[i], ";") && strcmp(argv[i], "|"))
			i++;
		if (!strcmp(*argv, "cd"))
			g_exit = ft_cd(argv, i);
		else if (i)
			g_exit = ft_exec(argv, env, i);
	}
	return (g_exit);
}
