/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 18:44:48 by iouardi           #+#    #+#             */
/*   Updated: 2022/03/22 21:27:36 by iouardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (*(s + i))
		i++;
	return (i);
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	src_length;
	size_t	dst_length;
	size_t	dsst_length;
	size_t	i;

	if (!dst && !size)
		return (ft_strlen(src));
	src_length = ft_strlen(src);
	dst_length = ft_strlen(dst);
	dsst_length = src_length;
	i = 0;
	if (dst_length < size)
		dsst_length += dst_length;
	else
		dsst_length += size;
	while (src[i] && dst_length + 1 < size)
	{
		dst[dst_length++] = src[i++];
	}
	dst[dst_length] = '\0';
	return (dsst_length);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*arr;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (strdup(s2));
	if (!s2)
		return (strdup(s1));
	arr = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (arr == NULL)
		return (NULL);
	arr[0] = '\0';
	ft_strlcat(arr, s1, ft_strlen(s1) + 1);
	ft_strlcat(arr, s2, ft_strlen(s1) + ft_strlen(s2) + 1);
	return (arr);
}

void	*ft_memset(void *b, int c, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		((unsigned char *)b)[i] = c;
		i++;
	}
	return (b);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = malloc(size * count);
	if (ptr == NULL)
		return (NULL);
	ft_memset(ptr, 0, count * size);
	return (ptr);
}

int	count_words(char const *s, char c)
{
	int		compt;
	int		i;

	compt = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			compt++;
		i++;
	}
	return (compt);
}

char	**length_words(char const *s, char c, char n, char **arr)
{
	int		j;
	int		i;
	int		k;
	int		l;
	int		r;

	j = 0;
	i = -1;
	while (s[j] && ++i < n)
	{
		while (s[j] && s[j] == c)
			if (!s[j++])
				return (NULL);
		l = 0;
		k = 0;
		r = j;
		while (s[j] && s[j++] != c)
			k++;
		arr[i] = ft_calloc(k + 1, sizeof(char));
		while (l < k)
			arr[i][l++] = s[r++];
	}
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	int		count;

	if (!s)
		return (NULL);
	count = count_words(s, c);
	arr = (char **) malloc((count + 1) * sizeof(char *));
	if (arr == NULL)
	{
		free(arr);
		return (NULL);
	}
	arr = length_words(s, c, count, arr);
	arr[count] = NULL;
	return (arr);
}

char	**berhouch_khames(char **env, char	*cmd)
{
	int i = 0;
	char	**line;
	while (env[i])
	{
		if (!strncmp(env[i], "PATH", 4))
			line = ft_split(env[i] + 5, ':');
		i++;
	}
	i = 0;
	while (line[i])
	{
		line[i] = ft_strjoin(line[i], "/");
		line[i] = ft_strjoin(line[i], cmd);
		i++;
	}
	return (line);
}

char	*check_path(char **line, char *cmd)
{
	int		i = 0;

	while (line[i])
	{
		if (!access(line[i], F_OK))
			return (line[i]);
		i++;
	}
	return (NULL);
}

int main(int argc, char **argv, char **env)
{
	int		pid;
	int 	fd1 = open(argv[1], O_RDONLY);
	int 	fd2 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	int 	i = 0;
	int		p[2];
	char	buffer[1];
	int		buffer_len;
	char	**arr1;

	if (pipe(p) < 0)
		exit(1);
	char **line = berhouch_khames(env, argv[2]);
	char *path = check_path(line, argv[2]);
	pid = fork();
	if (pid == -1)
		return (0);
	if (pid == 0)
	{
		dup2(fd1, 0);
		dup2(p[1], 1);
		close(p[1]);
		arr1 = ft_split(argv[2], ' ');
		execve(path, arr1, env);
	}
	waitpid(0, NULL, 0);
	pid = fork();
	if (pid == -1)
		return (0);
	char **arr2 = ft_split(argv[3], ' ');
	line = berhouch_khames(env, arr2[0]);
	path = check_path(line, arr2[0]);
	
	if (pid == 0)
	{
		dup2(fd2, 1);
		dup2(p[0], 0);
		// buffer_len = 1;
		// while (buffer_len)
		// {
		// buffer_len = read (p[0], buffer, 100000);
		// printf ("%s", buffer);
			
		// }
		close(p[0]);
		// printf("ay haja\n");
		execve(path, arr2, env);
		printf("rip\n");
	}
	waitpid(pid,NULL,0);
	printf("chi haja\n");
	return (0);
}