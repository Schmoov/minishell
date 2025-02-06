/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parden <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:10:50 by parden            #+#    #+#             */
/*   Updated: 2025/02/06 20:11:47 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLTIN_H
# define BLTIN_H

typedef struct s_minishell	t_ms;

int	blt_export_str(t_ms *ms, char *s);
int	blt_export(t_ms *ms, char **argv);
int	blt_env(t_ms *ms, char **argv);
int	blt_unset(t_ms *ms, char **argv);
int	blt_echo(char **argv);
int	blt_cd(char **argv);
int	blt_pwd(void);
int	blt_exit(t_ms *ms);

#endif
