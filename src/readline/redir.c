/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:10:47 by parden            #+#    #+#             */
/*   Updated: 2025/02/12 19:25:24 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

int	redir_hd(char *delim, t_ms *ms)
{
	static int	idx = 0;
	int			fd;
	char		name[16];
	char		*line;
	int			tmp;

	strcpy(name, "/tmp/mshd_");
	name[9] = (idx / 10) - '0';
	name[10] = (idx % 10) - '0';
	name[11] = 0;
	fd = open(name, O_RDWR | O_CREAT | O_TRUNC);
	tmp = dup(ms->fd[0]);
	dup2(ms->fd[0], STDIN_FILENO);
	line = readline("heredoc>");
	while (line && ft_strcmp(line, delim))
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		line = readline("heredoc>");
	}
	close(fd);
	fd = open(name, O_RDONLY | O_CREAT);
	idx = (idx + 1) % 100;
	return (fd);
}
