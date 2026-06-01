/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caida-si <caida-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 11:02:38 by caida-si          #+#    #+#             */
/*   Updated: 2025/07/17 12:47:35 by caida-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*gnl_join_and_free(char *s1, char *s2)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	if (s1)
		free(s1);
	return (res);
}

static char	*gnl_read_append(int fd, char *bk)
{
	char	*buf;
	ssize_t	bytes;

	buf = (char *)malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	bytes = 1;
	while (bytes > 0 && !ft_strchr(bk, '\n'))
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes < 0)
		{
			free(buf);
			free(bk);
			return (NULL);
		}
		buf[bytes] = '\0';
		bk = gnl_join_and_free(bk, buf);
		if (!bk)
		{
			free(buf);
			return (NULL);
		}
	}
	return (free(buf), (bk));
}

static char	*gnl_extract_line(char *bk)
{
	size_t	i;
	char	*line;

	i = 0;
	if (!bk || !*bk)
		return (NULL);
	while (bk[i] && bk[i] != '\n')
		i++;
	if (bk[i] == '\n')
		i++;
	line = ft_substr(bk, 0, i);
	return (line);
}

static char	*gnl_new_backup(char *bk)
{
	size_t	i;
	char	*new_bk;

	i = 0;
	if (!bk)
		return (NULL);
	while (bk[i] && bk[i] != '\n')
		i++;
	if (!bk[i])
	{
		free(bk);
		return (NULL);
	}
	new_bk = ft_strdup(bk + i + 1);
	free(bk);
	if (new_bk && !*new_bk)
	{
		free(new_bk);
		return (NULL);
	}
	return (new_bk);
}

char	*get_next_line(int fd)
{
	static char	*backup;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	backup = gnl_read_append(fd, backup);
	if (!backup)
		return (NULL);
	line = gnl_extract_line(backup);
	backup = gnl_new_backup(backup);
	if (!line && backup)
	{
		free(backup);
		backup = NULL;
	}
	return (line);
}
