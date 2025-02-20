/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:10:47 by parden            #+#    #+#             */
/*   Updated: 2025/02/20 17:24:41 by lscheupl         ###   ########.fr       */
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
	int tmp;

	strcpy(name, "/tmp/mshd_");
	name[9] = (idx / 100) + '0';
	name[10] = (idx / 10) + '0';
	name[11] = (idx % 10) + '0';
	name[12] = '\0';
	dprintf(2, "name: %s\n", name);
	fd = open(name, O_RDWR | O_CREAT | O_TRUNC, 0666);
	tmp = dup(STDIN_FILENO);
	dup2(ms->fd[0], STDIN_FILENO);
	line = readline("heredoc>");
	while (line && ft_strcmp(line, delim))
	{
		write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
        line = readline("heredoc> ");
	}
	free(line);
	close(fd);
	dup2(tmp, STDIN_FILENO);
	close(tmp);
    fd = open(name, O_RDONLY | O_CREAT, 0666);
    idx = (idx + 1) % 1000;
	return (fd);
}
