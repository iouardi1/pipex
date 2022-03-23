/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 15:58:01 by iouardi           #+#    #+#             */
/*   Updated: 2022/03/23 19:45:40 by iouardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char    *find_path(char *cmd, char **env)
{
	int     i = 0;
	char    *paths;
	char	**path = NULL;

	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH=", 5))
		{
			paths = ft_strdup(ft_strnstr(env[i], "PATH=", 5));
			path = ft_split(paths, ':');
			break ;
		}
		i++;
	}
	i = 0;
	while (path[i])
	{
		path[i] = ft_strjoin(path[i], "/");
		path[i] = ft_strjoin(path[i], cmd);
		if (!access(path[i], F_OK))
			return (path[i]);
		i++;
	}
	return (NULL);
}

int	execute_cmd1(char *path, char **cmd1, char  *argv, int *p, char **env)
{
	int		pid;
	int		fd1 = open(argv, O_RDONLY, 0666);

	pid = fork();
	if  (pid == -1)
		return (2);
	if (pid == 0)
	{
		dup2(fd1, 0);
		dup2(p[1], 1);
		close (p[1]);
		execve(path, cmd1, env);
	}
	return (0);
}

int	execute_cmd2(char *path, char **cmd2, char *argv, int *p, char **env)
{
	int		pid;
	int		fd2 = open(argv, O_WRONLY | O_TRUNC | O_CREAT, 0777);

	pid = fork();
	if  (pid == -1)
		return (2);
	if (pid == 0)
	{
		dup2(p[0], 0);
		dup2(fd2, 1);
		close (p[1]);
		close (p[0]);
		execve(path, cmd2, env);
	}
	wait(NULL);
	return (0);
}

int main (int argc, char **argv, char **env)
{
	char	**cmd1 = NULL;
	char	**cmd2 = NULL;
	char	*path = NULL;
	int		p[2];

	if (argc == 5)
	{
		cmd1 = ft_split(argv[2], ' ');
		cmd2 = ft_split(argv[3], ' ');
		path = find_path (cmd1[0], env);
		if (pipe(p) == -1)
			return (2);
		
		if (execute_cmd1(path, cmd1, argv[1], p, env) == 2)
			write(2, "pipe failed my dear :(\n", 24);
		path = find_path (cmd2[0], env);
		if (execute_cmd2(path, cmd2, argv[4], p, env) == 2)
			write(2, "pipe failed my dear :(\n", 24);
	}
	else
		write(2, "please insert two commmands and a file to read from :) tyt no pressure ^^\n", 75);
}