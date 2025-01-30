/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:11:55 by lscheupl          #+#    #+#             */
/*   Updated: 2025/01/29 19:17:08 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void    ft_echo(char **args)
{
    int i;

    i = 1;
    while (args[i])
    {
        if (i != 1)
            write(1, " ", 1);
        write(1, args[i], ft_strlen(args[i]));
        i++;
    }
    write(1, "\n", 1);
}

void    ft_exec_builtin(t_node_cmd *node,t_ms *ms)
{
    if (node->bltin == E_ECHO)
        ft_echo(node->args);
    // else if (node->bltin == E_CD)
    //     ft_cd(node->args, ms);
    // else if (node->bltin == E_PWD)
    //     ft_pwd();
    // else if (node->bltin == E_EXPORT)
    //     ft_export(node->args, ms);
    // else if (node->bltin == E_UNSET)
    //     ft_unset(node->args, ms);
    // else if (node->bltin == E_ENV)
    //     ft_env(ms);
    // else if (node->bltin == E_EXIT)
    //     ft_exit(node->args, ms);
}

