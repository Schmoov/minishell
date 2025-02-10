/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leonel <leonel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:41:00 by leonel            #+#    #+#             */
/*   Updated: 2025/02/07 17:40:24 by leonel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <stdio.h>



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
			if (fd == -1)
				printf("error\n");  
			redir[0] = fd;
			spl_remove(args, i + 1);
			spl_remove(args, i);
		}
		}
		i++;
	}
}

int exec_builtin(t_node_cmd **node, t_ms *ms)
{
	if ((*node)->bltin == E_EXIT)
		exit_exec(ms);
	else if ((*node)->bltin == E_CD)
		return (blt_cd((*node)->args));
	else if ((*node)->bltin == E_PWD)
		return (blt_pwd());
	else if ((*node)->bltin == E_ECHO)
		return (blt_echo((*node)->args));
	else if ((*node)->bltin == E_EXPORT)
		return (blt_export(ms, (*node)->args));
	else if ((*node)->bltin == E_UNSET)
		return (blt_unset(ms, (*node)->args));
	else if ((*node)->bltin == E_ENV)
		return (blt_env(ms, (*node)->args));
	return (0);
}
int is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "exit", 4) == 0)
		return (E_EXIT);
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		return (E_CD);
	else if (ft_strncmp(cmd, "pwd", 3) == 0)
		return (E_PWD);
	else if (ft_strncmp(cmd, "echo", 4) == 0)
		return (E_ECHO);
	else if (ft_strncmp(cmd, "export", 6) == 0)
		return (E_EXPORT);
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		return (E_UNSET);
	else if (ft_strncmp(cmd, "env", 3) == 0)
		return (E_ENV);
	return (E_NOTBLTIN);
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
			free(node->pip_redir);
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
