/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 15:58:01 by iouardi           #+#    #+#             */
/*   Updated: 2022/04/06 02:23:39 by iouardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	print_error(char *cmd)
{
	char	*err;

	err = strerror (errno);
	ft_putstr_fd("shell: ", 2);
	ft_putstr_fd(err, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putchar_fd('\n', 2);
}

int	check_path(char *path)
{
	if (!access (path, F_OK))
		return (0);
	else if (ft_strchr (path, '/'))
		return (2);
	return (1);
}

void	execute_cmd_(t_pipexa piipe, char **env)
{
	if (!piipe.path)
	{
		write(2, "command not found\n", 19);
		exit(1);
	}
	close(piipe.p[0]);
	dup2(piipe.p[1], 1);
	close(piipe.p[1]);
	execve(piipe.path, piipe.cmd, env);
	print_error(piipe.cmd[0]);
	exit(1);
}

int	execute_cmd(t_pipexa piipe, char *argv, char **env)
{
	int		pid;

	if (pipe(piipe.p) == -1)
		exit (1);
	pid = fork();
	if (pid == -1)
		exit (1);
	piipe.cmd = ft_split(argv, ' ');
	if (!check_path(piipe.cmd[0]) || check_path(piipe.cmd[0]) == 2)
		piipe.path = ft_strdup(piipe.cmd[0]);
	else
		piipe.path = find_path (piipe.cmd[0], env);
	if (pid == 0)
		execute_cmd_(piipe, env);
	else
	{
		close(piipe.p[1]);
		dup2(piipe.p[0], 0);
		close(piipe.p[0]);
	}
	return (pid);
}

void	close_n_wait(t_pipexa piipe, int *pid)
{
	int		i;

	i = 0;
	close(piipe.p[0]);
	close(piipe.p[1]);
	while (pid[i])
	{
		waitpid(pid[i], NULL, 0);
		i++;
	}
}

void	check_fd(int fd)
{
	if (fd == -1)
	{
		write(2, "no such file or directory\n", 27);
		exit(1);
	}
}

int	check_here_doc(char *argv)
{
	int		i;

	i = ft_strlen("here_doc");
	if (!ft_strncmp(argv, "here_doc", i))
		return (1);
	return (0);
}

void	last_command_(t_pipexa piipe, int fd, char **env, char *path_temp)
{
	dup2(fd, 1);
	execve(piipe.path, piipe.cmd, env);
	free(path_temp);
}

int	last_command(t_pipexa piipe, char **argv, int argc, char **env)
{
	int		fd;
	char	*path_temp;
	int		pid;

	piipe.cmd = ft_split(argv[argc - 2], ' ');
	fd = 0;
	piipe.path = find_path(piipe.cmd[0], env);
	path_temp = piipe.path;
	if (!piipe.path)
	{
		write(2, "Command not found\n", 19);
		exit (2);
	}
	if (check_here_doc(argv[1]))
		fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
	else
		fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	check_fd(fd);
	pid = fork();
	if (pid == -1)
		write (2, "fork failed\n", 13);
	if (pid == 0)
		last_command_(piipe, fd, env, path_temp);
	return (pid);
}

void	type_line_n_check_argv(t_pipexa piipe, char *line, char *argv, int len)
{
	while (1)
	{
		line = get_next_line(0);
		if (!ft_strncmp(line, argv, len) && line[len] == '\n')
			exit(0);
		write(piipe.p[1], line, ft_strlen(line));
	}
}

void	for_my_dear_here_doc(char *argv, t_pipexa piipe)
{
	int		pid;
	char	*line;
	int		len;

	if (pipe(piipe.p) == -1)
		exit (1);
	pid = fork();
	line = NULL;
	if (pid == -1)
		write(2, "fork error\n", 12);
	if (pid == 0)
	{
		len = ft_strlen(argv);
		type_line_n_check_argv(piipe, line, argv, len);
		close (piipe.p[1]);
	}
	else
	{
		dup2(piipe.p[0], 0);
		close(piipe.p[0]);
		close(piipe.p[1]);
		waitpid(pid, NULL, 0);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_pipexa	piipe;
	int			i;
	int			j;
	int			*pid;
	int			fd1;

	i = 2;
	j = 0;
	pid = malloc((argc - 3) * sizeof(int));
	fd1 = open (argv[1], O_RDONLY, 0666);
	piipe.path = NULL;
	if (argc >= 5)
	{
		if (check_here_doc(argv[1]))
		{
			for_my_dear_here_doc(argv[2], piipe);
			i = 3;
		}
		else
		{
			if (fd1 < 0)
			{
				i = 3;
				write(2, "shell: no such file or directory\n", 34);
			}
			dup2(fd1, 0);
		}
		while (i < argc - 2)
			pid[j++] = execute_cmd(piipe, argv[i++], env);
		pid[j] = last_command(piipe, argv, argc, env);
		if (pid[j] == 2)
			exit(1);
		close_n_wait(piipe, pid);
	}
	else
		write(2, "please insert at least two commands\n", 37);
}
