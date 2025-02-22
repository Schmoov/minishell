/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:10:47 by parden            #+#    #+#             */
/*   Updated: 2025/02/22 16:06:54 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <unistd.h>

int	redir_in(char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	return (fd);
}

int	redir_out(char *path)
{
	int	fd;

	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	return (fd);
}

int	redir_app(char *path)
{
	int	fd;

	fd = open(path, O_WRONLY | O_APPEND | O_CREAT, 0666);
	return (fd);
}

int	heredoc_parse(char *delim, t_ms *ms)
{
	static int	idx = 0;
	int			fd;
	char		name[16];
	char		*line;

	strcpy(name, "/tmp/mshd_");
	name[10] = (idx / 10) + '0';
	name[11] = (idx % 10) + '0';
	name[12] = 0;
	fd = open(name, O_RDWR | O_CREAT | O_TRUNC, 0666);
	line = readline("heredoc>");
	while (line && ft_strcmp(line, delim))
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline("heredoc>");
	}
	free(line);
	close(fd);
	idx = (idx + 1) % 100;
	if (idx)
		return (idx - 1);
	return (99);
}

void	heredoc_process(t_ms *ms, int *pos)
{
	int		l;
	int		r;
	int		idx;
	char	*tmp;

	r = *pos + 2;
	while (ms->input[r] == ' ' || ms->input[r] == '\t')
			r++;
	l = r;
	while (ms->input[r] && ms->input[r] != ' ' && ms->input[r] != '\t')
	{
		if (ms->input[r] == '\'' || ms->input[r] == '"')
			r = close_quote(ms->input, r, ft_strlen(ms->input));
		else
			r++;
	}
	tmp = ft_substr(ms->input, l, r - l);
	idx = heredoc_parse(tmp, ms);
	free(tmp);
	tmp = calloc(ft_strlen(ms->input) + 16, 1);
	strncat(tmp, ms->input, *pos + 1);
	strcat(tmp, "/tmp/mshd_");
	tmp[ft_strlen(tmp)] = idx / 10 + '0';
	tmp[ft_strlen(tmp)] = idx % 10 + '0';
	strcat(tmp, ms->input + r);
	*pos += 13;
	free(ms->input);
	ms->input = tmp;
	dprintf(2, ms->input);
	dprintf(2, "\n");
}
