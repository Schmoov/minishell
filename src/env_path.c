#include "../inc/minishell.h"

char	**spldup(char **spl)
{
	int		i;
	char	**res;

	i = 0;
	while (spl[i])
		i++;
	res = ft_calloc(i + 1, sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (spl[i])
	{
		res[i] = ft_strdup(spl[i]);
		if (!res[i])
			return (ft_free_split(spl), NULL);
		i++;
	}
	return (res);
}

void	spl_append(char ***spl, char *new)
{
	int		i;
	char	**res;

	i = 0;
	while ((*spl)[i])
		i++;
	res = ft_calloc(i + 2, sizeof(char *));
	if (!res)
		return (ft_free_split(*spl));
	i = 0;
	while ((*spl)[i])
	{
		res[i] = ft_strdup((*spl)[i]);
		if (!res[i])
			return (ft_free_split(*spl), ft_free_split(res));
		i++;
	}
	res[i] = ft_strdup(new);
	if (!res[i])
		return (ft_free_split(*spl), ft_free_split(res));
	ft_free_split(*spl);
	*spl = res;
}

//Finds a match on the len first char
int	spl_find(char **spl, char *s, int len)
{
	int	i;

	i = 0;
	while (spl[i])
	{
		if (!ft_strncmp(spl[i], s, len))
			return (i);
		i++;
	}
	return (-1);
}

void	spl_replace(char **spl, char *new, int pos)
{
	char	*dup;

	dup = ft_strdup(new);
	if (!dup)
		ft_free_split(spl);
	free(spl[pos]);
	spl[pos] = dup;
}

int	blt_export_unvalid_id(char *s)
{
	int	s_len;

	s_len = ft_strlen(s);
	write(2, "export: '", 9);
	write(2, s, s_len);
	write(2, "' not a valid identifier\n", 25);
	return (1);
}

int	blt_export_str(t_ms *ms, char *s)
{
	int	eq_pos;
	int	env_pos;

	if (!*s)
		return (0);
	if (!(ft_isalpha(*s) || *s == '_'))
		return (blt_export_unvalid_id(s));
	eq_pos = 1;
	while (s[eq_pos] && s[eq_pos] != '=')
	{
		if (!(ft_isalnum(s[eq_pos]) || s[eq_pos] == '_'))
			return (blt_export_unvalid_id(s));
		eq_pos++;
	}
	if (!s[eq_pos])
		return (0);
	env_pos = spl_find(ms->envp, s, eq_pos + 1);
	if (env_pos == -1)
		spl_append(&ms->envp, s);
	else
		spl_replace(ms->envp, s, env_pos);
	return (0);
}

int	blt_export(t_ms *ms, char **argv)
{
	if (!argv[1] || argv[2])
	{
		write(2, "export expects a single argument\n", 33);
		return (1);
	}
	return (blt_export_str(ms, argv[1]));
}
