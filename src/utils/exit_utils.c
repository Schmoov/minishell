/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leonel <leonel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 20:19:41 by parden            #+#    #+#             */
/*   Updated: 2025/02/10 23:24:32 by leonel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	exit_alloc(t_ms *ms)
{
	ms_destroy(ms);
	exit(1);
}

int	exit_exec(t_ms *ms)
{
	ms_destroy(ms);
	exit(1);
}

int	exit_pipe(t_ms *ms)
{
	ms_destroy(ms);
	exit(1);
}

int	exit_fork(t_ms *ms)
{
	ms_destroy(ms);
	exit(1);
}
