/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:41:00 by leonel            #+#    #+#             */
/*   Updated: 2025/01/29 18:56:39 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <stdio.h>

void	ft_clean_path(char **tab_path)
{
	int	i;
	int	j;

	i = 0;
	j = ft_strlen(tab_path[0]) - 5;
	while (i < j)
	{
		tab_path[0][i] = tab_path[0][i + 5];
		i++;
	}
}

char	*ft_convert_pos_to_string(char *input, int start, int end)
{
	char	*res;
	int		i;
	int		j;

	res = ft_calloc((end - start + 1), sizeof(char));
	i = 0;
	j = start;
	while (i < end - start)
	{
		res[i++] = input[j++];
	}
	res[end - start] = '\0';
	return (res);
}
char *ft_strndup(const char *s, size_t n)
{
	char	*dup;

	dup = (char *)malloc(n + 1 * sizeof(char));
	if (!dup)
		return (NULL);
	return ((char *)ft_memcpy(dup, s, n));
}

size_t where_is_dollar(char *input, size_t i)
{
	while (input[i])
	{
		if (input[i] == '$')
			return (i);
		i++;
	}
	return (i);
}

size_t where_dollar_end(char *input, size_t i)
{
	i++;
	while (input[i] != ' ' && input[i] != '\0' && input[i] != '$' && input[i] != '\"' && input[i] != '\'') //white space, end of string, dollar sign, double quote, single quote  
		i++;
	return (i);
}

char *conversion_dollar(char *input, t_ms *ms)
{
	int		i;
	int		j;
	char	*res;

	i = 0;
	j = 0;
	while (ms->envp[i])
	{
		if (ft_strncmp(ms->envp[i], input, ft_strlen(input)) == 0)
		{
			if (ms->envp[i][ft_strlen(input)] == '=')
			{
				res = ft_convert_pos_to_string(ms->envp[i], ft_strlen(input) + 1, ft_strlen(ms->envp[i]));
				return (res);
			}
		}
		i++;
	}
	res = ft_strdup("");
	return (res);
}

char *dollar_expander(char *input, t_ms *ms, int *index)
{
	int		i;
	char	*res;
	char	*tmp;
	char	*tmp2;
	
	res = NULL;
	tmp = NULL;
	tmp2 = NULL;
	i = where_is_dollar(input, *index);
	res = ft_strndup(input, i);
	tmp = ft_convert_pos_to_string(input, i + 1, where_dollar_end(input, i));
	tmp2 = conversion_dollar(tmp, ms);
	*index += ft_strlen(tmp2);
	tmp = ft_strjoin(res, tmp2);
	free(res);
	free(tmp2);
	res = ft_strjoin(tmp, input + where_dollar_end(input, i));
	free(tmp);
	return (res);
}

int	skip_single_quote(char *res, int i)
{
	i++;
	while (res[i] != '\'')
		i++;
	return (i);
}

void	ft_clean_digit_dollar(char *res, int i)
{
	int j;

	j = i + 2;
	while (res[j] != '\0')
	{
		res[j - 2] = res[j];
		j++;
	}
	res[j - 2] = '\0';
}

int	is_in_quote(char *res, int i)
{
	if (res[i - 1] == '\"' && res[i + 1] == '\"')
		return (1);
	return (0);
}

void	ft_clean_interrogation(char *res, int i)
{
	int j;

	j = i + 1;
	while (res[j] != '\0')
	{
		res[j - 1] = res[j];
		j++;
	}
	res[j - 1] = '\0';
}

char	*ft_expander(char *input, int start, int end, t_ms *ms)
{
	char	*res;
	int i;
	int double_quote;

	i = 0;
	double_quote = 0;
	res = ft_convert_pos_to_string(input, start, end);
	while (res[i])
	{
		if (res[i] == 34)
			double_quote++;
		if (res[i] == '\'' && double_quote%2 == 0)
			i = skip_single_quote(res, i);
		if (res[i] == '$')
		{
			if (isdigit(res[i + 1]))
				ft_clean_digit_dollar(res, i);
			else if (is_in_quote(res, i) == 1)
				i++;
			else if (res[i + 1] == '?')
			{
				res[i] = ms->status + '0';
				ft_clean_interrogation(res, i + 1);
			}
			else
			{
			res = dollar_expander(res, ms, &i);
			i--;
			}
		}
		i++;
	}
	return (res);
}
char	**ft_isolate_path(t_ms *ms)
{
	char	*path;
	char	**tab_path;
	int		i;

	i = 0;
	while (ms->envp[i])
	{
		if (ms->envp[i][0] == 'P' && ms->envp[i][1] == 'A'
			&& ms->envp[i][2] == 'T' && ms->envp[i][3] == 'H')
			path = ft_strdup(ms->envp[i]);
		i++;
	}
	tab_path = ft_split(path, ':');
	ft_clean_path(tab_path);
	free(path);
	return (tab_path);
}

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

char	*ft_find_path(char *expanded, t_ms *ms, char **args)
{
	char		**tab_path;
	char		*bin;
	char		*cmd;

	bin = NULL;
	// root = &ms->ast->cmd;
	tab_path = ft_isolate_path(ms);
	bin = ft_check_access(tab_path, args[0]);
	ft_free_split(tab_path);
	return (bin);
}

void	ft_execve(char *path, char **tab_arg, char **envp)
{
	execve(path, tab_arg, envp);
	perror("execve");
	exit(EXIT_FAILURE);
}

void	exec_cmd(char *input, t_ast *root, t_ms *ms)
{
	int			i;
	t_node_cmd	*node;
	char		**tab_arg;
	char		*path;
	pid_t		pid;

	node = &(root->cmd);
	node->expanded = ft_expander(input, node->start, node->end, ms);
	node->args = ft_split(node->expanded, ' ');
	path = ft_find_path(node->expanded, ms, node->args);
	tab_arg = ft_split(node->expanded, 32);
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