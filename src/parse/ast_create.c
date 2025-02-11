/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_create.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:02:18 by linux             #+#    #+#             */
/*   Updated: 2025/02/11 16:52:27 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//pointers that are yet to be allocated are initialized to NULL
//so the freeing process can be agnostic to where it is called

t_ast	*ast_create_cmd(int start, int end)
{
	t_ast		*res;
	t_node_cmd	*node;

	res = ft_calloc(1, sizeof(t_ast));
	if (!res)
		return (NULL);
	res->type = E_CMD;
	node = &res->cmd;
	node->start = start;
	node->end = end;
	node->redir[0] = -1;
	node->redir[1] = -1;
	return (res);
}

t_ast	*ast_create_logic(int start, int end, bool is_and)
{
	t_ast			*res;
	t_node_logic	*node;

	res = ft_calloc(1, sizeof(t_ast));
	if (!res)
		return (NULL);
	res->type = E_LOGIC;
	node = &res->logic;
	node->start = start;
	node->end = end;
	node->is_and = is_and;
	return (res);
}

t_ast	*ast_create_pip(int start, int end, int pip_len)
{
	t_ast		*res;
	t_node_pip	*node;

	res = ft_calloc(1, sizeof(t_ast));
	if (!res)
		return (NULL);
	res->type = E_PIP;
	node = &(res->pip);
	node->start = start;
	node->end = end;
	node->pip_len = pip_len;
	node->piped = ft_calloc(pip_len, sizeof(t_ast *));
	if (!node->piped)
		return (free(res), NULL);
	return (res);
}

t_ast	*ast_create_grp(int start, int end)
{
	t_ast			*res;
	t_node_grp		*node;

	res = ft_calloc(1, sizeof(t_ast));
	if (!res)
		return (NULL);
	res->type = E_GRP;
	node = &res->grp;
	node->start = start;
	node->end = end;
	return (res);
}
