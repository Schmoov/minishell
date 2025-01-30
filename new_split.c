/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 17:58:43 by lscheupl          #+#    #+#             */
/*   Updated: 2025/01/30 18:39:13 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "inc/minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>


static size_t	ft_strlen(const char *s)
{
    char	*p;

    p = (char *)s;
    while (*p)
        p++;
    return (p - s);
}

static size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	if (!dstsize)
		return (ft_strlen(src));
	i = 0;
	while (src[i] && i < dstsize - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = 0;
	while (src[i])
		i++;
	return (i);
}

static char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ret;

	ret = (char *)malloc((len + 1) * sizeof(char));
	if (!ret)
		return (NULL);
	ft_strlcpy(ret, s + start, len + 1);
	return (ret);
}

static void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*str;
	size_t			i;

	str = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		str[i] = (unsigned char)c;
		i++;
	}
	return ((void *)str);
}

static void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}

static void	*ft_calloc(size_t nelem, size_t elsize)
{
	void	*ret;

	if (elsize && 10000000 / elsize < nelem)
	{
		return (NULL);
	}
	ret = (void *)malloc(nelem * elsize);
	if (!ret)
		return (NULL);
	ft_bzero(ret, nelem * elsize);
	return (ret);
}

static void	free_res(char **res)
{
	size_t	i;

	i = 0;
	while (res[i])
	{
		free(res[i]);
		i++;
	}
	free(res);
}

static size_t	count_words(char *s, char c)
{
	size_t	count;
    int i = 0;

	count = 0;
	while (s[i])
	{
        if (s[i] == 34)
        {
            printf("prout\n");
            i++;
            while (s[i] != '"')
                i++;
            if (s[i])
            {
			count++;
            printf("count\": %zu\n", count);
            }
            // s++;
        }
        if (s[i] == '\'')
        {
             printf("prout\n");
            i++;
            while (s[i] != '\'')
                i++;
            if (s[i])
            {
			count++;
            printf("count\': %zu\n", count);
            }
            // s++;
        }
		while (s[i] == c)
			i++;
		if (s[i])
        {
			count++;
            printf("countc: %zu\n", count);
        }
		while (s[i] && s[i] != c && s[i] != 34 && s[i] != 39)
		    i++;
	}
	return (count);
}

//Returns the end of current word or 0 if alloc fails
static size_t	insert_word(char **res, char const *s, size_t start, char c)
{
	size_t	len;
    int single_quote;
    int double_quote;


    single_quote = 0;
    double_quote = 0;
	while (s[start] == c)
		start++;
    if (s[start] == '\"')
    {
        start++;
        double_quote++;
    }
    else if (s[start] == '\'')
    {
        start++;
        single_quote++;
    }
	len = 0;
    if (double_quote % 2 == 1)
    {
        while (s[start + len] && s[start + len] != '\"')
            len++;
    }
    else if (single_quote % 2 == 1)
    {
        while (s[start + len] && s[start + len] != '\'')
            len++;
    }
    else
    {
        while (s[start + len] && s[start + len] != c)
            len++;
    }
	*res = ft_substr(s, start, len);
	if (!(*res))
		return (0);
	return (start + len);
}

static char	**wrap_split(char **res, char const *s, char c, size_t count)
{
	size_t	start;
	size_t	i;

	start = 0;
	i = 0;
	while (i < count)
	{
		start = insert_word(res + i, s, start, c);
		if (!start)
		{
			free_res(res);
			return (NULL);
		}
		i++;
	}
	return (res);
}

char	**ft_new_split(char *s, char c)
{
	size_t	word_count;
	char	**res;

	word_count = count_words(s, c);
    printf("word_count: %zu\n", word_count);
	res = (char **)ft_calloc(word_count + 1, sizeof(char *));
	if (!res)
		return (NULL);
	return (wrap_split(res, s, c, word_count));
}
int main()
{
    char *str = "ls -l \"hello world\"";
    char **tab = ft_new_split(str, ' ');
    int i = 0;
    while (tab[i])
    {
        printf("%s\n", tab[i]);
        i++;
    }
    return (0);
}