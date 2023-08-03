/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 15:37:44 by bluzi             #+#    #+#             */
/*   Updated: 2023/07/31 00:32:48 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pre_num_split(char const *s, char c)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	if (!s)
		return (0);
	while (s[i] == c && s[i])
		i++;
	while (s[i])
	{		
		if (s[i] != c)
		{
			n++;
			while (s[i] != c && s[i])
				i++;
		}
		else
			i++;
	}
	return (n);
}

int	len_word_split(char const *s, char c)
{
	int	j;

	j = 0;
	while (s[j] != c && s[j])
		j++;
	return (j);
}

void	fill_split(char *src, char *dst, int len)
{	
	int	i;

	i = 0;
	while (i < len)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

char	**split(t_shell *shell, char const *s, char c)
{
	int		i;
	int		m;
	char	**spl;

	if (!s)
		return (NULL);
	m = 0;
	i = 0;
	spl = gc_malloc(shell, (pre_num_split(s, c) + 1) * sizeof(char *));
	while (m < pre_num_split(s, c))
	{
		while (s[i] == c)
			i++;
		spl[m] = gc_malloc(shell, (len_word_split(s + i, c) + 1));
		fill_split((char *)(s + i), spl[m++], len_word_split(s + i, c));
		i = i + len_word_split(s + i, c);
	}
	spl[m] = NULL;
	return (spl);
}
