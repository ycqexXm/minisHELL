/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 14:30:41 by bluzi             #+#    #+#             */
/*   Updated: 2023/07/31 16:23:02 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_process(t_shell *shell, t_execution *block, int n)
{
	char	**command;
	int		exit_code;
	char	*executable;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	piping(shell, n);
	if (block->command && is_builtin(shell, block->command))
	{
		exit_code = execute_builtin(shell, block->redir, block->command);
		free_garbage(shell->garbage);
		exit(exit_code);
	}
	redirs(shell, block->redir);
	if (block->command)
	{	
		command = list_to_array(shell, block->command);
		executable = find_executable(shell, command[0], shell->paths);
		execve(executable, command, shell->env);
	}
	free_garbage(shell->garbage);
	exit(EXIT_SUCCESS);
}

void	close_and_wait(t_shell *shell)
{
	int	i;

	i = -1;
	while (++i < shell->pipes_count)
	{
		close(shell->pipes[i][READ_END]);
		close(shell->pipes[i][WRITE_END]);
	}
	i = -1;
	while (++i < shell->pipes_count + 1)
		waitpid(shell->pids[i], &g_exit_code, 0);
	if (WIFEXITED(g_exit_code))
		g_exit_code = WEXITSTATUS(g_exit_code);
}

void	allocate_pipes_and_pids(t_shell *shell)
{
	shell->pids = gc_malloc(shell, (shell->pipes_count + 1) * sizeof(pid_t));
	if (shell->pipes_count > 0)
		shell->pipes = gc_malloc(shell,
				(shell->pipes_count) * sizeof(int *));
}

void	execution(t_shell *shell, t_execution *execution_list)
{
	int	n;

	n = 0;
	get_paths(shell, shell->env);
	if (shell->pipes_count == 0 && is_builtin(shell, execution_list->command))
		g_exit_code = execute_builtin(shell, execution_list->redir,
				execution_list->command);
	else
	{
		allocate_pipes_and_pids(shell);
		while (execution_list)
		{
			get_paths(shell, shell->env);
			if (n < shell->pipes_count)
				shell->pipes[n] = gc_malloc(shell, 2 * sizeof(int));
			if (n < shell->pipes_count)
				pipe(shell->pipes[n]);
			shell->pids[n] = fork();
			if (shell->pids[n] == 0)
				child_process(shell, execution_list, n);
			execution_list = execution_list->next;
			n++;
		}
		close_and_wait(shell);
	}
}
