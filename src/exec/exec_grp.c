/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_grp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 17:08:57 by lscheupl          #+#    #+#             */
/*   Updated: 2025/02/12 17:09:06 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	exec_grp(char *input, t_ast *root, t_ms *ms)
{
	t_node_grp	*node;

	node = &(root->grp);
	return (exec_general(input, node->next, ms));
}
