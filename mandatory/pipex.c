/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 15:58:01 by iouardi           #+#    #+#             */
/*   Updated: 2022/03/28 23:33:35 by iouardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_free(char **arr)
{
	int		i;

	i = 0;
	while (arr[i])
	{
		free (arr[i]);
		i++;
	}
	free (arr);
}

int	execute_cmd1(t_pipexa piipe, char *argv, char **env)
{
	int		pid;
	int		pid1;
	int		fd1;

	fd1 = open(argv, O_RDONLY, 0666);
	pid = fork();
	if (pid == -1)
		return (2);
	if (pid == 0)
	{
		pid1 = pid;
		dup2(fd1, 0);
		dup2(piipe.p[1], 1);
		close (piipe.p[0]);
		execve(piipe.path, piipe.cmd1, env);
	}
	return (pid);
}

int	execute_cmd2(t_pipexa piipe, char *argv, char **env)
{
	int		pid;
	int		fd2;

	fd2 = open(argv, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	pid = fork();
	if (pid == -1)
		return (2);
	if (pid == 0)
	{
		dup2(piipe.p[0], 0);
		dup2(fd2, 1);
		close (piipe.p[1]);
		close (piipe.p[0]);
		execve(piipe.path, piipe.cmd2, env);
	}
	return (pid);
}

void	close_n_wait(t_pipexa piipe, int pid1, int pid2)
{
	close(piipe.p[0]);
	close(piipe.p[1]);  
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}

int	main(int argc, char **argv, char **env)
{
	t_pipexa	piipe;
	char		*path_temp;
	int			pid1;
	int			pid2;

	if (argc == 5)
	{
		piipe.cmd1 = ft_split(argv[2], ' ');
		piipe.cmd2 = ft_split(argv[3], ' ');
		piipe.path = find_path (piipe.cmd1[0], env);
		path_temp = piipe.path;
		if (pipe(piipe.p) == -1)
			return (2);
		pid1 = execute_cmd1(piipe, argv[1], env);
		free (path_temp);
		if (pid1 == 2)
			write(2, "pipe failed my dear :(\n", 24);
		piipe.path = find_path (piipe.cmd2[0], env);
		pid2 = execute_cmd2(piipe, argv[4], env);
		if (pid2 == 2)
			write(2, "pipe failed my dear :(\n", 24);
		close_n_wait(piipe, pid1, pid2);
	}
	else
		write(2, "please insert two commmands\n", 29);
}
