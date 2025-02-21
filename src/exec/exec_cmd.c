/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 17:33:31 by leonel            #+#    #+#             */
/*   Updated: 2025/02/21 17:41:14 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_execve(char *path, t_node_cmd *node, t_ms *ms)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	redir_executions(node->redir, ms);
	close_all(node->redir, ms);
	execve(path, node->args, ms->envp);
	perror("execve");
	ms->status = 126;
	exit_exec(ms);
}

void	redir_executions(int *redir, t_ms *ms)
{
	if (redir[0] != -1)
	{
		// dup2(ms->fd[0], STDIN_FILENO);
		dup2(redir[0], STDIN_FILENO);
		close(redir[0]);
	}
	if (redir[1] != -1)
	{
		dup2(redir[1], STDOUT_FILENO);
		close(redir[1]);
	}
}

void	close_all(int *redir, t_ms *ms)
{
	if (redir[0] != -1)
		close(redir[0]);
	if (redir[1] != -1)
		close(redir[1]);
	redir[0] = -1;
	redir[1] = -1;
	ms_close_fd(ms);
}

char *get_next_word(char *input, int i, t_node_cmd *node)
{
	int j;
	
	while (input[i] == ' ')
		i++;
	j = i;
	while (input[i] != ' ' && input[i] != '\0')
		i++;
	node->start = i;
	return (pos_to_string(input, j, node->start));
}
int redir(char *word, int fd[2], t_ms *ms, int type)
{
	word = ft_expander(word, ms);
	single_layer_quotes_remover(word);
	if (type == 0)
		fd[0] = redir_in(word);
	else if (type == 1)
		fd[1] = redir_out(word);
	else if (type == 2)
		fd[1] = redir_app(word);
	else if (type == 3)
	{
		fd[0] = redir_hd(word, ms);
		// dup2(fd[0], STDIN_FILENO);
		// close(fd[0]);
		// fd[0] = -1;
	}
	free(word);
	if (fd[0] == -1 && fd[1] == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}


int	handle_redir_before(char *input, t_node_cmd *node, t_ms *ms)
{
	int	i;

	i = node->start;
	while (i < node->end && input[i] == ' ')
		i++;
	if (input[i] == '>')
	{
		if (input[i + 1] == '>')
		{
			if (redir(get_next_word(input, i + 2, node), node->redir, ms, 2)) // leaks here
				return (ms->status = 1, EXIT_FAILURE);
		}
		else
		{
			if (redir(get_next_word(input, i + 1, node), node->redir, ms, 1))// leaks here
				return (ms->status = 1, EXIT_FAILURE);
		}
	}
	else if (input[i] == '<')
	{
		if (input[i + 1] == '<')
		{
			if (redir(get_next_word(input, i + 2, node), node->redir, ms, 3))// leaks here
				return (ms->status = 1, EXIT_FAILURE);
		}
		else
		{
			if (redir(get_next_word(input, i + 1, node), node->redir, ms, 0))// leaks here
				return (ms->status = 1, EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

int	is_redir_before(char *input, t_node_cmd *node)
{
	int	i;

	i = node->start;
	while (i < node->end && input[i] == ' ')
		i++;
	if (input[i] == '>' || input[i] == '<')
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int	exec_cmd(char *input, t_ast *root, t_ms *ms)
{
	t_node_cmd	*node;
	char		*path;
	pid_t		pid;

	node = &(root->cmd);
	path = NULL;
	while (is_redir_before(input, node) == EXIT_SUCCESS)
	{
		if (handle_redir_before(input, node, ms) != EXIT_SUCCESS)
			return (write(2, "redir failed\n", 13), close_all(node->redir, ms), ms->status = 1);
	}
	node->args = to_expansion(pos_to_string(input, node->start, node->end), ms);
	if (redir_handler(node->redir, node->args, ms) != 0)
		return (write(2, "redir failed\n", 13), close_all(node->redir, ms), ms->status = 1);
	if (is_builtin(node->args[0]) != E_NOTBLTIN)
		return (exec_builtin(node, ms));
	if (ft_strncmp(node->args[0], "./", 2) == 0 || ft_strncmp(node->args[0], "/", 1) == 0)
		path = ft_strdup(node->args[0]);
	else
		path = ft_find_path(ms, node->args);
	if (path == NULL)
		return (fprintf(stderr, "command not found\n"),
			close_all(node->redir, ms), ms->status = 127);
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
