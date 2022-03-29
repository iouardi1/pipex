/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 15:58:01 by iouardi           #+#    #+#             */
/*   Updated: 2022/03/29 03:08:52 by iouardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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

int	execute_cmd(t_pipexa piipe, char *argv, char **env, int i)
{
	int	pid;

	piipe.cmd = ft_split(argv, ' ');
	piipe.path = find_path(piipe.cmd[0], env);
	pid = fork();
	if (pid == -1)
		return (2);
	if (pipe(piipe.p) == -1)
			return (2);
	if (pid == 0)
	{
		printf("%d\n", i);
		if (i > 2)
			dup2(piipe.p[0], 0);
		dup2(piipe.p[1], 1);
		// close(piipe.p[0]);
		// close(piipe.p[1]);
		execve(piipe.path, piipe.cmd, env);
	}
	return (pid);
}

void	close_n_wait(t_pipexa piipe, int *pid)
{
	int		i = 0;
	close(piipe.p[0]);
	close(piipe.p[1]);
	while (pid[i])
	{	
		waitpid(pid[i], NULL, 0);
		i++;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_pipexa	piipe;
	int			pid[argc - 3];
	int			fd1 = open (argv[1], O_RDONLY, 0666);
	int			fd2 = open (argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0777);
	int			i = 2;
	int			j = 0;

	piipe.path = NULL;
	if (argc >= 5)
	{
		// pid[j] = fork();
		// if (pid[j] == -1)
		// 	return (2);
		// if (pid[j] == 0)
		// {
		// 	if (pipe(piipe.p) )
		// 	piipe.cmd = ft_split(argv[i], ' ');
		// 	piipe.path = find_path(piipe.cmd[0], env);
		// 	dup2(fd1, 0);
		// 	dup2(piipe.p[1], 1);
		// 	execve(piipe.path, piipe.cmd, env);
		// }
		dup2(fd1, 0);
		while (argv[i + 2])
		{
			pid[j++] = execute_cmd(piipe, argv[i], env, i);
			i++;
		}
		pid[j] = fork();
		if (pid[j] == -1)
			return 2;
		dup2(piipe.p[0], 0);
		if (pid[j] == 0)
		{
			piipe.cmd = ft_split(argv[i], ' ');
			piipe.path = find_path(piipe.cmd[0], env);
			dup2(fd2, 1);
			execve(piipe.path, piipe.cmd, env);
		}
		// close_n_wait(piipe, pid);
	}
	else
		write(2, "please insert at least two commmands\n", 29);
}
