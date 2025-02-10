/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leonel <leonel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 17:31:51 by leonel            #+#    #+#             */
/*   Updated: 2025/02/10 18:57:27 by leonel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_check_access(char **tab_path, char *cmd)
{
	char	test[255];
	char	*res;
	int		i;

	i = 0;
	res = NULL;
	if (access(cmd, X_OK) == 0)
		return (res = ft_strdup(cmd));
	while (tab_path[i])
	{
		ft_strlcpy(test, tab_path[i], 256);
		ft_strlcat(test, "/", 256);
		ft_strlcat(test, cmd, 256);
		if (access(test, X_OK) == 0)
			break ;
		ft_bzero(test, 256);
		i++;
	}
	if (access(test, X_OK) == 0)
	{
		res = malloc(sizeof(char) * (ft_strlen(test) + 1));
		ft_strlcpy(res, test, (ft_strlen(test) + 1));
	}
	return (res);
}

char	*ft_isolate_first_word(char *expanded)
{
	int		i;
	char	*cmd;

	i = 0;
	while (expanded[i] != ' ' && expanded[i] != '\0')
	{
		i++;
	}
	cmd = malloc(sizeof(char) * i + 2);
	i = 0;
	while (expanded[i] != ' ' && expanded[i] != '\0')
	{
		cmd[i] = expanded[i];
		i++;
	}
	cmd[i] = '\0';
	return (cmd);
}

char	*ft_find_path(t_ms *ms, char **args)
{
	char	**tab_path;
	char	*bin;

	bin = NULL;
	tab_path = ft_isolate_path(ms);
	bin = ft_check_access(tab_path, args[0]);
	ft_free_split(tab_path);
	return (bin);
}

void	ms_fd(t_ms *ms)
{
	ms->fd[0] = dup(STDIN_FILENO);
	ms->fd[1] = dup(STDOUT_FILENO);
}

void	ms_close_fd(t_ms *ms)
{
	close(ms->fd[0]);
	close(ms->fd[1]);
}
