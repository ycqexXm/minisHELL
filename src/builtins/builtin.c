/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 01:20:48 by yorlians          #+#    #+#             */
/*   Updated: 2023/07/31 05:56:38 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(t_shell *shell, t_token *node)
{
	char	*command;

	if (!node)
		return (0);
	command = string_duplicate(shell, node -> token);
	if (string_compare(command, "echo") == 0)
		return (gc_free(shell, command), node->id = ECO, 1);
	else if (string_compare(command, "cd") == 0)
		return (gc_free(shell, command), node->id = CD, 1);
	else if (string_compare(command, "pwd") == 0)
		return (gc_free(shell, command), node->id = PWD, 1);
	else if (string_compare(command, "export") == 0)
		return (gc_free(shell, command), node->id = EXPORT, 1);
	else if (string_compare(command, "unset") == 0)
		return (gc_free(shell, command), node->id = UNSET, 1);
	else if (string_compare(command, "env") == 0)
		return (gc_free(shell, command), node->id = ENV, 1);
	else if (string_compare(command, "exit") == 0)
		return (gc_free(shell, command), node->id = EXIT, 1);
	else
		return (0);
}

int	execute_builtin(t_shell *shell, t_token	*redir, t_token *command)
{
	int		exit_code;
	int		std_fd[2];

	std_fd[0] = dup(0);
	std_fd[1] = dup(1);
	exit_code = 0;
	redirs(shell, redir);
	if (command->id == ECO)
		exit_code = echo(shell, list_to_array(shell, command));
	else if (command->id == CD)
		exit_code = cd(shell, list_to_array(shell, command), shell->env);
	else if (command->id == PWD)
		exit_code = pwd();
	else if (command->id == EXPORT)
		exit_code = export(shell, command);
	else if (command->id == UNSET)
		exit_code = unset(shell, command);
	else if (command->id == ENV)
		exit_code = env(list_to_array(shell, command), shell->env);
	else if (command->id == EXIT)
		exit_code = shell_exit(shell, list_to_array(shell, command));
	restore_std_fd(std_fd);
	return (exit_code);
}

void	restore_std_fd(int *std_fd)
{
	dup2(std_fd[0], 0);
	dup2(std_fd[1], 1);
	close(std_fd[0]);
	close(std_fd[1]);
}
