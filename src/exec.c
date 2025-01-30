/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:41:00 by leonel            #+#    #+#             */
/*   Updated: 2025/01/30 16:15:19 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <stdio.h>

void	ft_execve(char *path, char **tab_arg, char **envp)
{
	execve(path, tab_arg, envp);
	perror("execve");
	exit(EXIT_FAILURE);
}

void	exec_cmd(char *input, t_ast *root, t_ms *ms)
{
	t_node_cmd	*node;
	char		**tab_arg;
	char		*path;
	pid_t		pid;

	node = &(root->cmd);
	node->expanded = ft_expander(input, node->start, node->end, ms);
	node->args = ft_split(node->expanded, ' ');
	tab_arg = ft_split(node->expanded, 32);
	if (node->bltin != E_NOTBLTIN)
	{
		// ft_exec_builtin(&node, ms);
		return;
	}
	path = ft_find_path(node->expanded, ms, node->args);
	if (path == NULL)
	{
		fprintf(stderr, "command not found: %s\n", node->args[0]);
		ms->status = 127;
		return;
	}
	pid = fork();
	if (pid == -1)
	{
        perror("fork");
		ms->status = 1;
        return;
    }
	if (pid == 0)
	{
		ft_execve(path, tab_arg, ms->envp);
		ms->status = 1;
		exit(EXIT_FAILURE);
	}
	waitpid(pid, NULL, 0);
	free(path);
	ft_free_split(tab_arg);
	ms->status = 0;
}

void	exec_pip(char *input, t_ast *root, t_ms *ms)
{
	int			i;
	int			j;
	t_node_pip	*node;
	pid_t		pid;

	i = 0;
	j = 0;
	node = &(root->pip);
	node->pip_redir = malloc((sizeof (int[2]) * node->pip_len));
	while (i < node->pip_len)
	{
		if (pipe(node->pip_redir[i]) == -1)
		{
			perror("pipe");
			return;
		}
		i++;
	}
	i = 0;
	while (i < node->pip_len)
    {
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return;
		}
		if (pid == 0)
		{
			if (i != 0)
			{
				dup2(node->pip_redir[i - 1][0], STDIN_FILENO);
				close(node->pip_redir[i - 1][0]);
			}
			if (i != node->pip_len - 1)
			{
				dup2(node->pip_redir[i][1], STDOUT_FILENO);
				close(node->pip_redir[i][1]);
			}
			while (j < node->pip_len)
			{
				close(node->pip_redir[j][0]);
				close(node->pip_redir[j][1]);
				j++;
			}
	        exec_general(input, node->piped[i], ms);
			exit(EXIT_FAILURE);
		}
		i++;
    }
	j = 0;
	while (j < node->pip_len)
	{
		close(node->pip_redir[j][0]);
		close(node->pip_redir[j][1]);
		j++;
	}
	i = 0;
	while (i < node->pip_len)
	{
		waitpid(pid, NULL, 0);
		i++;
	}
	free(node->pip_redir);
}

void	exec_grp(char *input, t_ast *root, t_ms *ms)
{
	int			i;
	t_node_grp	*node;

	i = 0;
	node = &(root->grp);
	printf("grp\n");
	exec_general(input, node->next, ms);
}

void	exec_logic(char *input, t_ast *root, t_ms *ms)
{
	int				i;
	t_node_logic	*node;

	i = 0;
	node = &(root->logic);
	printf("logic\n");
	exec_general(input, node->left, ms);
	fprintf(stderr, "status: %d\n", ms->status);
	if (node->is_and)
	{
		if (ms->status == 0)
		{
			exec_general(input, node->right, ms);
		}
	}
	else
	{
		if (ms->status == 1)
			exec_general(input, node->right, ms);
	}
	printf("logic\n");
}
void	exec_general(char *input, t_ast *root, t_ms *ms)
{
	t_node_type node_type;

	node_type = root->type;
	if (node_type == E_CMD)
		exec_cmd(input, root, ms);
	else if (node_type == E_LOGIC)
		exec_logic(input, root, ms);
	else if (node_type == E_PIP)
		exec_pip(input, root, ms);
	else
		exec_grp(input, root, ms);
}