/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:38:57 by bluzi             #+#    #+#             */
/*   Updated: 2023/07/30 00:44:26 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_alpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}

int	is_alpha_num(int c)
{
	if (is_alpha(c) || (c >= '0' && c <= '9'))
		return (1);
	return (0);
}

int	is_space(int character)
{
	character = (unsigned char)character;
	if (character == '\t' || character == '\n' || character == '\v'
		|| character == '\f' || character == '\r' || character == ' ')
		return (1);
	return (0);
}

int	is_separator(char *c)
{
	if (*c == '|')
		return (1);
	if (*c == '>')
		return (1 + (*(c + 1) == '>'));
	if (*c == '<')
		return (1 + (*(c + 1) == '<'));
	return (0);
}
