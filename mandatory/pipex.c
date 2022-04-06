/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 15:58:01 by iouardi           #+#    #+#             */
/*   Updated: 2022/04/05 18:28:56 by iouardi          ###   ########.fr       */
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

int	check_path(char *path)
{
	if (!access (path, F_OK))
		return (0);
	else if (ft_strchr (path, '/'))
		return (2);
	return (1);
}

void	print_error(char *cmd)
{
	char	*err = strerror (errno);
	ft_putstr_fd("shell: ", 2);
	ft_putstr_fd(err, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putchar_fd('\n', 2);
}

int	execute_cmd1(t_pipexa piipe, char *argv, char **env)
{
	int		pid;
	int		fd1;

	fd1 = open(argv, O_RDONLY, 0666);
	if (fd1 < 0)
	{
		write(2, "no such file or directory\n", 27);
		return 1;
	}
	pid = fork();
	if (pid == -1)
		return (2);
	if (pid == 0)
	{
		if (!piipe.path)
		{
			write(2, "command not found\n", 19);
			exit (1);
		}
		dup2(fd1, 0);
		dup2(piipe.p[1], 1);
		close (piipe.p[0]);
		execve(piipe.path, piipe.cmd1, env);
		print_error(piipe.cmd1[0]);
		exit (1);
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
		if (!piipe.path)
		{
			write(2, "command not found\n", 19);
			exit (1);
		}
		dup2(piipe.p[0], 0);
		dup2(fd2, 1);
		close (piipe.p[1]);
		execve(piipe.path, piipe.cmd2, env);
		// write (2, "command not found: ", 20);
		print_error(piipe.cmd2[0]);
		return 1;
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
		if (!check_path(piipe.cmd1[0]) || check_path(piipe.cmd1[0]) == 2)
			piipe.path = ft_strdup(piipe.cmd1[0]);
		else
			piipe.path = find_path (piipe.cmd1[0], env);
		path_temp = piipe.path;
		if (pipe(piipe.p) == -1)
			exit(1);
		pid1 = execute_cmd1(piipe, argv[1], env);
		free (path_temp);
		if (pid1 == 2)
			exit(1);
		if (!check_path(piipe.cmd2[0]) || check_path(piipe.cmd2[0]) == 2)
			piipe.path = ft_strdup(piipe.cmd2[0]);
		else
			piipe.path = find_path (piipe.cmd2[0], env);
		pid2 = execute_cmd2(piipe, argv[4], env);
		if (pid2 == 2)
			exit(1);
		close_n_wait(piipe, pid1, pid2);
	}
	else
		write(2, "please insert two commmands\n", 29);
}
