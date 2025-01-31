/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:41:00 by leonel            #+#    #+#             */
/*   Updated: 2025/01/31 16:34:39 by lscheupl         ###   ########.fr       */
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

char	*quote_remover(char *str)
{
	int		i;
	int		j;
	char	*res;

	i = 0;
	j = 0;
	res = malloc(sizeof(char) * (ft_strlen(str) + 1));
	while (str[i])
	{
		if (str[i] == '\'')
		{
			i++;
			while (str[i] != '\'')
			{
				res[j] = str[i];
				i++;
				j++;
			}
			i++;
		}
		if (str[i] == '\"')
		{
			i++;
			while (str[i] != '\"')
			{
				res[j] = str[i];
				i++;
				j++;
			}
			i++;
		}
		res[j] = str[i];
		j++;
		i++;
	}
	res[j] = '\0';
	free(str);
	return (res);
}

int	exec_cmd(char *input, t_ast *root, t_ms *ms)
{
	t_node_cmd	*node;
	char		*path;
	pid_t		pid;
	int wstatus;

	node = &(root->cmd);
	//gerer redirection
	//hors redir
	node->args = ft_expander(input, node->start, node->end, ms);
	// node->expanded = quote_remover(node->expanded);
	// printf("expanded: %s\n", node->expanded);
	// node->args = ft_split(node->expanded, ' ');
	// tab_arg = ft_split(node->expanded, 32);
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
        return (exit_fork(ms));
    }
	if (pid == 0)
	{
		ft_execve(path, node->args, ms->envp);
		exit_exec(ms);
	}
	waitpid(pid, &wstatus, 0);
	if (WIFSIGNALED(wstatus))
	{
		return (128 + WTERMSIG(wstatus));
	}
	free(path);
	ft_free_split(node->args);
	return(WEXITSTATUS(wstatus));
}

int	exec_pip(char *input, t_ast *root, t_ms *ms)
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
			return (exit_pipe(ms));
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
			return (exit_fork(ms));
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
	        return (exec_general(input, node->piped[i], ms));
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

int	exec_grp(char *input, t_ast *root, t_ms *ms)
{
	int			i;
	t_node_grp	*node;

	i = 0;
	node = &(root->grp);
	return (exec_general(input, node->next, ms));
}

int	exec_logic(char *input, t_ast *root, t_ms *ms)
{
	int				i;
	t_node_logic	*node;
	int status;

	i = 0;
	node = &(root->logic);
	status = exec_general(input, node->left, ms);
	if (node->is_and)
	{
		if (status == 0)
		{
			return (exec_general(input, node->right, ms));
		}
	}
	else
	{
		if (status == 1)
			return (exec_general(input, node->right, ms));
	}
}
int	exec_general(char *input, t_ast *root, t_ms *ms)
{
	t_node_type node_type;

	node_type = root->type;
	if (node_type == E_CMD)
		return (ms->status = exec_cmd(input, root, ms));
	else if (node_type == E_LOGIC)
		return (ms->status =exec_logic(input, root, ms));
	else if (node_type == E_PIP)
		return (ms->status =exec_pip(input, root, ms));
	else
		return (ms->status =exec_grp(input, root, ms));
}