/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 20:23:54 by yorlians          #+#    #+#             */
/*   Updated: 2023/07/31 16:23:02 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_exit_syntax(t_shell *shell, char *arguments)
{
	int	j;

	j = 0;
	while (arguments[j] || j == 0)
	{
		if (!isdigit(arguments[j]) && !(j == 0 && arguments[1]
				&& (arguments[0] == '-' || arguments[0] == '+')))
		{
			write(2, "minishell: exit: ", 18);
			write(2, arguments, str_len(arguments));
			write(2, ": numeric argument required\n", 29);
			free_garbage(shell -> garbage);
			exit(255);
		}
		j++;
	}
}

int	shell_exit(t_shell *shell, char **arguments)
{
	int	i;
	int	exit_status;

	write(2, "exit\n", 6);
	i = 0;
	while (arguments[i])
		i++;
	if (i == 1)
	{
		exit_status = g_exit_code;
		free_garbage(shell->garbage);
		exit(exit_status);
	}
	else
	{
		check_exit_syntax(shell, arguments[1]);
		if (i == 2)
		{
			exit_status = atoi(arguments[1]) % 256;
			free_garbage(shell->garbage);
			exit(exit_status);
		}
		write(STDERR_FILENO, "minishell: exit: too many arguments\n", 37);
	}
	return (1);
}
