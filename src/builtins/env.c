/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 20:23:33 by yorlians          #+#    #+#             */
/*   Updated: 2023/07/31 00:09:51 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env(char **arguments, char **env)
{
	int	i;

	i = 0;
	while (arguments[i])
	{
		if (arguments[i][0] == '-')
		{
			write(2, "env: ", 5);
			write(2, arguments[1], str_len(arguments[1]));
			write(2, ": illegal option\n", 18);
			return (EXIT_FAILURE);
		}
		i++;
	}
	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}
