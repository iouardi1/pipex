/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 15:58:31 by iouardi           #+#    #+#             */
/*   Updated: 2022/04/06 23:26:28 by iouardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <stdlib.h>
# include <errno.h>
# include "get_next_line.h"
# include "libft/libft.h"

typedef struct s_pipexa
{
	char	*path;
	char	**cmd;
	int		p[2];
}	t_pipexa;

// char	*ft_strnstr(const char *str1, const char *str2, size_t len);
// char	*ft_strjoin(char const *s1, char const *s2);
// char	**ft_split(char const *s, char c);
// char	*ft_strdup(const char *s1);
char	*find_path(char *cmd, char **env);
// int		ft_memcmp(const void *s1, const void *s2, size_t n);
// char	*ft_substr(char const *s, unsigned int start, size_t len);
// size_t	ft_strlen(const char *s);
// int		ft_strncmp(const char *s1, const char *s2, size_t n);
// char	*get_next_line(int fd);
int		check_here_doc(char *argv);
void	type_line_n_check_argv(t_pipexa piipe, char *line, char *argv, int len);
void	for_my_dear_here_doc(char *argv, t_pipexa piipe);
void	if_its_here_doc_or_not(t_pipexa piipe, char **argv, int *i, int fd1);
void	execute_cmd_(t_pipexa piipe, char **env);
int		execute_cmd(t_pipexa piipe, char *argv, char **env);
void	last_command_(t_pipexa piipe, int fd, char **env, char *path_temp);
int		last_command(t_pipexa piipe, char **argv, int argc, char **env);
void	check_fd1(int fd1, int *i);
void	check_fd(int fd);
void	print_error(char *cmd);
int		check_path(char *path);
void	close_n_wait(t_pipexa piipe, int *pid);
void	if_its_here_doc_or_not(t_pipexa piipe, char **argv, int *i, int fd1);

#endif