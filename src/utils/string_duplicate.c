/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_duplicate.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:24:57 by bluzi             #+#    #+#             */
/*   Updated: 2023/07/31 00:09:51 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*string_duplicate(t_shell *shell, char *string)
{
	int		i;
	int		length;
	char	*copy;

	if (!string)
		return (NULL);
	length = 0;
	while (string[length])
		length++;
	copy = gc_malloc(shell, sizeof(char) * (length + 1));
	i = -1;
	while (string[++i])
		copy[i] = string[i];
	copy[i] = '\0';
	return (copy);
}

char	*str_n_dup(t_shell *shell, const char *string, int n)
{
	char	*copy;
	int		i;

	i = 0;
	if (str_len(string) < n)
		copy = gc_malloc(shell, sizeof(char) * (str_len(string) + 1));
	else
		copy = gc_malloc(shell, sizeof(char) * (n + 1));
	while (string[i] && i < n)
	{
		copy[i] = string[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

char	*string_join(t_shell *shell, char *str1, char *str2)
{
	int		i;
	int		y;
	int		size;
	char	*joined;

	i = 0;
	y = 0;
	size = 0;
	if (!str1 && !str2)
		return (NULL);
	if (str1)
		size += str_len(str1);
	if (str2)
		size += str_len(str2);
	joined = gc_malloc(shell, size + 1);
	while (str1 && str1[y])
		joined[i++] = str1[y++];
	y = 0;
	while (str2 && str2[y])
		joined[i++] = str2[y++];
	joined[i] = '\0';
	return (joined);
}
