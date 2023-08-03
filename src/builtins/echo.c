/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bluzi <bluzi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 01:36:47 by yorlians          #+#    #+#             */
/*   Updated: 2023/07/31 16:06:05 by bluzi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	option_n_check(char	*string)
{
	int	i;

	if (!string || str_len(string) < 2)
		return (0);
	if (string[0] != '-')
		return (0);
	i = 0;
	while (string[++i])
	{
		if (string[i] != 'n')
			return (0);
	}
	return (1);
}

int	echo(t_shell *shell, char **command)
{
	int	i;
	int	option_n;

	i = 1;
	option_n = 1;
	while (command[i] && option_n_check(command[i]))
	{
		option_n = 0;
		i++;
	}
	while (command[i])
	{
		write(STDOUT_FILENO, command[i], str_len(command[i]));
		if (command[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (option_n)
		write(STDOUT_FILENO, "\n", 1);
	return (free_duoble_ptr(shell, command), EXIT_SUCCESS);
}
