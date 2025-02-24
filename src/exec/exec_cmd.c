/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 17:33:31 by leonel            #+#    #+#             */
/*   Updated: 2025/02/24 20:19:08 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_execve(char *path, t_node_cmd *node, t_ms *ms)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	redir_executions(node->redir);
	close_all(node->redir, ms);
	execve(path, node->args, ms->envp);
	perror("execve");
	ms->status = 126;
	exit_exec(ms);
}

int	handle_redir_before(char *input, t_node_cmd *node, t_ms *ms)
{
	int		i;
	char	*word;

	i = node->start;
	while (i < node->end && input[i] == ' ')
		i++;
	if (input[i] == '>')
	{
		if (input[i + 1] == '>')
		{
			word = get_next_word(input, i + 2, node);
			return (ms->status = redir(word, node->redir, ms, 2));
		}
		else
		{
			word = get_next_word(input, i + 1, node);
			return (ms->status = redir(word, node->redir, ms, 1));
		}
	}
	else if (input[i] == '<')
	{
		word = get_next_word(input, i + 1, node);
		return (ms->status = redir(word, node->redir, ms, 0));
	}
	return (EXIT_SUCCESS);
}

int	redir_cmd(char *input, t_node_cmd *node, t_ms *ms)
{
	int		i;
	char	*tmp;

	i = 0;
	while (is_redir_before(input, node) == EXIT_SUCCESS)
	{
		if (handle_redir_before(input, node, ms) != EXIT_SUCCESS)
			return (close_all(node->redir, ms), ms->status = 1);
	}
	tmp = ft_substr(input, node->start, node->end);
	if (node->start >= node->end || is_empty(tmp))
	{
		node->args = ft_calloc(1, sizeof(char *));
		return (free(tmp), close_all(node->redir, ms), EXIT_SUCCESS);
	}
	node->args = to_expansion(pos_to_string(input, node->start, node->end), ms);
	if (redir_handler(node->redir, node->args) != 0)
		return (free(tmp), close_all(node->redir, ms), ms->status = 1);
	while (node->args[i])
		single_layer_quotes_remover(node->args[i++]);
	return (free(tmp), EXIT_SUCCESS);
}

int	path_cmd(t_node_cmd *node, char **path, t_ms *ms)
{
	if (ft_strncmp(node->args[0], "./", 2) == 0 || ft_strncmp(node->args[0],
			"/", 1) == 0)
		(*path) = ft_strdup(node->args[0]);
	else
		(*path) = ft_find_path(ms, node->args);
	if ((*path) == NULL)
		return (close_all(node->redir, ms), ms->status = 127);
	return (EXIT_SUCCESS);
}

int	exec_cmd(char *input, t_ast *root, t_ms *ms)
{
	t_node_cmd	*node;
	char		*path;
	pid_t		pid;

	node = &(root->cmd);
	path = NULL;
	if (redir_cmd(input, node, ms) != EXIT_SUCCESS)
		return (write(2, "redir failed\n", 13), EXIT_FAILURE);
	if (is_builtin(node->args[0]) != E_NOTBLTIN)
		return (exec_builtin(node, ms));
	if (path_cmd(node, &path, ms) != EXIT_SUCCESS)
		return (write(2, "command not found\n", 19), ms->status);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), exit_fork(ms));
	if (pid == 0)
		ft_execve(path, node, ms);
	dup2(ms->fd[0], STDIN_FILENO);
	dup2(ms->fd[1], STDOUT_FILENO);
	close_all(node->redir, ms);
	waitpid(pid, &ms->status, 0);
	free(path);
	if (WIFSIGNALED(ms->status))
		return (128 + WTERMSIG(ms->status));
	return (WEXITSTATUS(ms->status));
}
