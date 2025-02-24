/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 20:19:41 by parden            #+#    #+#             */
/*   Updated: 2025/02/24 19:38:55 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	exit_alloc(t_ms *ms)
{
	ms_destroy(ms);
	exit(ms->status);
}

int	exit_exec(t_ms *ms)
{
	ms_destroy(ms);
	exit(ms->status);
}

int	exit_pipe(t_ms *ms)
{
	ms_destroy(ms);
	exit(ms->status);
}

int	exit_fork(t_ms *ms)
{
	ms_destroy(ms);
	exit(ms->status);
}
