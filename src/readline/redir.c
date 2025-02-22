/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:10:47 by parden            #+#    #+#             */
/*   Updated: 2025/02/22 17:14:52 by parden           ###   ########.fr       */
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

int	heredoc_parse(char *delim)
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

void	heredoc_process_helper(t_ms *ms, int *l, int *r)
{
	while (ms->input[*r] == ' ' || ms->input[*r] == '\t')
			(*r)++;
	*l = *r;
	while (ms->input[*r] && ms->input[*r] != ' ' && ms->input[*r] != '\t')
	{
		if (ms->input[*r] == '\'' || ms->input[*r] == '"')
			*r = close_quote(ms->input, *r, ft_strlen(ms->input));
		else
			(*r)++;
	}
}
