/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 15:05:58 by bluzi             #+#    #+#             */
/*   Updated: 2023/07/31 06:52:35 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_directory(char *path)
{
	struct stat	path_stat;

	stat(path, &path_stat);
	return (S_ISDIR(path_stat.st_mode));
}

void	print_is_dir(char *command)
{
	write(2, "minishell: ", str_len("minishell: "));
	write(2, command, str_len(command));
	write(2, ": is a directory\n", str_len(": is a directory\n"));
}

int	exist_and_exec(t_shell *shell, char *command)
{
	if (!access(command, F_OK))
	{
		if (string_compare(command, "..") == 0
			|| string_compare(command, ".") == 0)
		{
			dup2(2, 1);
			printf("minishell: %s: %s", command, COMMAND_ERR);
			free_garbage(shell->garbage);
			exit(127);
		}
		if (is_directory(command))
		{
			print_is_dir(command);
			free_garbage(shell->garbage);
			exit(126);
		}
		if (!access(command, X_OK))
			return (1);
		dup2(2, 1);
		printf("minishell: %s: %s", command, NO_ACCESS);
		free_garbage(shell->garbage);
		exit(126);
	}
	return (0);
}

void	print_command_err(t_shell *shell, char *command, char *error_message)
{
	write(2, "minishell: ", 12);
	write(2, command, str_len(command));
	write(2, ": ", 2);
	write(2, error_message, str_len(error_message));
	free_garbage(shell->garbage);
	exit(127);
}

char	*find_executable(t_shell *shell, char *command, char **paths)
{
	char	*executable;

	if ((*command == '.' || find_char(command, '/')))
	{
		if (exist_and_exec(shell, command))
			return (command);
		print_command_err(shell, command, COMMAND_ERR);
	}
	else
	{
		while (*paths)
		{
			executable = string_join(shell, *paths++, command);
			if (string_compare(command, "")
				&& exist_and_exec(shell, executable))
				return (executable);
			gc_free(shell, executable);
		}
		print_command_err(shell, command, COMMAND_ERR);
	}
	return (NULL);
}
