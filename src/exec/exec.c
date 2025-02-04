/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:41:00 by leonel            #+#    #+#             */
/*   Updated: 2025/02/04 20:30:03 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
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

void	redir_handler(int *redir, t_ms *ms, char **args)
{
	int i;
	int fd;

	i = 0;
	fd = -1;
	while (args[i] != NULL)
	{
		if (ft_strlen(args[i]) == 1)
		{
		if (ft_strncmp(args[i], ">", 1) == 0)
		{
			// printf("redir_out\n");
			fd = redir_out((args[i + 1]));
			if (fd == -1)
				printf("error\n");
			redir[1] = fd;
			spl_remove(args, i + 1);
			spl_remove(args, i);
		}
		else if (ft_strncmp(args[i], "<", 1) == 0)
		{
			// printf("redir_in\n");
			fd = redir_in(args[i + 1]);
			if (fd == -1)
				printf("error\n");
			redir[0] = fd;
			spl_remove(args, i + 1);
			spl_remove(args, i);
		}
		}
		else if (ft_strlen(args[i]) == 2)
		{
		if (ft_strncmp(args[i], ">>", 2) == 0) //append
		{
			// printf("redir_app\n");
			fd = redir_app(args[i + 1]);
			if (fd == -1)
				printf("error\n");
			redir[1] = fd;
			spl_remove(args, i + 1);
			spl_remove(args, i);
		}
		else if (ft_strncmp(args[i], "<<", 2) == 0) //heredoc
		{
			// printf("redir_hd\n");
			fd = redir_hd(args[i + 1]);
			// dup2(fd, ms->fd[0]);
			// // close(fd);
			redir[0] = fd;
			spl_remove(args, i + 1);
			spl_remove(args, i);
		}
		}
		i++;
	}
}

void redir_executions(int *redir, t_ms *ms)
{
	if (redir[0] != -1)
	{
		dup2(redir[0], ms->fd[0]);
		// close(redir[0]);
	}
	if (redir[1] != -1)
	{
		dup2(redir[1], ms->fd[1]);
		// close(redir[1]);
	}
}

int	exec_cmd(char *input, t_ast *root, t_ms *ms)
{
	t_node_cmd	*node;
	char		*path;
	pid_t		pid;

	node = &(root->cmd);
	node->args = expand_expand(ft_convert_pos_to_string(input, node->start, node->end), ms);
	node->redir[0] = -1;
	node->redir[1] = -1;
	redir_handler(node->redir, ms, node->args);
	// if (node->bltin != E_NOTBLTIN)
	// 	return(ft_exec_builtin(&node, ms));
	path = ft_find_path(node->expanded, ms, node->args);
	if (path == NULL)
		return (fprintf(stderr, "command not found: %s\n", node->args[0]), ms->status = 127);
	pid = fork();
	if (pid == -1)
	{
        perror("fork");
        return (/*exit_fork(ms)*/0);
    }
	if (pid == 0)
	{
		redir_executions(node->redir, ms);
		// if (node->redir[0] != -1)
        // close(node->redir[0]);
  		//   if (node->redir[1] != -1)
        // close(node->redir[1]);
		// close(ms->fd[0]);
		// close(ms->fd[1]);
		ft_execve(path, node->args, ms->envp);
		exit_exec(ms);
	}
	if (node->redir[0] != -1)
        close(node->redir[0]);
    if (node->redir[1] != -1)
        close(node->redir[1]);
	close(ms->fd[0]);
	close(ms->fd[1]);
	waitpid(pid, &ms->status, 0);
	free(path);
	
	if (WIFSIGNALED(ms->status))
		return (128 + WTERMSIG(ms->status));
	return(WEXITSTATUS(ms->status));
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
			return (exit_pipe(ms));
		}
		if (pid == 0)
		{
			if (i != 0)
			{
				dup2(node->pip_redir[i - 1][0],  STDIN_FILENO);
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
	        ms->status = exec_general(input, node->piped[i], ms);
			exit_pipe(ms);
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
	close(ms->fd[0]);
	close(ms->fd[1]);
	free(node->pip_redir);
	return (ms->status);
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
			status = exec_general(input, node->right, ms);
		}
	}
	else
	{
		if (status != 0)
			status = exec_general(input, node->right, ms);
	}
	return (status);
}
int	exec_general(char *input, t_ast *root, t_ms *ms)
{
	t_node_type node_type;

	node_type = root->type;
	if (node_type == E_CMD)
		return (ms->status = exec_cmd(input, root, ms));
	else if (node_type == E_LOGIC)
		return (ms->status = exec_logic(input, root, ms));
	else if (node_type == E_PIP)
		return (ms->status = exec_pip(input, root, ms));
	else
		return (ms->status = exec_grp(input, root, ms));
}