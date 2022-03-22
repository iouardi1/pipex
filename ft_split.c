/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/13 16:25:25 by iouardi           #+#    #+#             */
/*   Updated: 2022/03/22 16:00:31 by iouardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

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
