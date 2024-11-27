/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_destroy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linux <linux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:02:15 by linux             #+#    #+#             */
/*   Updated: 2024/11/27 18:38:33 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ast_destroy_cmd(t_ast *root)
{
	int	i;

	free(root->cmd.expanded);
	i = 0;
	while (root->cmd.args && root->cmd.args[i])
		free(root->cmd.args[i++]);
	free(root->cmd.args);
	free(root);
}

void	ast_destroy_logic(t_ast *root)
{
	ast_destroy(root->logic.left);
	ast_destroy(root->logic.right);
	free(root);
}

void	ast_destroy_pip(t_ast *root)
{
	int	i;

	i = 0;
	while (i < root->pip.pip_len)
		ast_destroy(root->pip.piped[i++]);
	free(root->pip.piped);
	free(root);
}

void	ast_destroy_grp(t_ast *root)
{
	ast_destroy(root->grp.next);
	free(root);
}

void	ast_destroy(t_ast *root)
{
	if (!root)
		return ;
	if (root->type == E_CMD)
		ast_destroy_cmd(root);
	else if (root->type == E_LOGIC)
		ast_destroy_logic(root);
	else if (root->type == E_PIP)
		ast_destroy_pip(root);
	else
		ast_destroy_grp(root);
}
