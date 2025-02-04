/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:10:47 by parden            #+#    #+#             */
/*   Updated: 2025/02/04 17:38:41 by lscheupl         ###   ########.fr       */
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
	fd = open(path, O_RDWR | O_CREAT, 0666);
	return (fd);
}

int	redir_app(char *path)
{
	int	fd;
	fd = open(path, O_RDWR | O_APPEND | O_CREAT);
	return (fd);
}

int	redir_hd(char *delim)
{
	static int	idx = 0;
	int			fd;
	char		name[16];
	char		*line;

	strcpy(name, "/tmp/mshd_");
	name[9] = (idx / 10) - '0';
	name[10] = (idx % 10) - '0';
	name[11] = 0;
	fd = open(name, O_RDWR | O_CREAT);
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
