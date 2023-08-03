/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 14:58:15 by bluzi             #+#    #+#             */
/*   Updated: 2023/07/31 16:23:02 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_code = 0;

void	reset_shell(t_shell *shell)
{
	destroy_heredocs(shell);
	if (shell->execution_list)
		free_execution_list(shell, shell->execution_list);
	shell->execution_list = NULL;
	if (shell->token_list)
		free_token_list(shell, shell->token_list);
	shell->token_list = NULL;
	shell->pids = NULL;
	shell->pipes = NULL;
	shell->pipes_count = 0;
	shell->heredoc_count = 0;
}

int	main(int argc, char **argv, char **env)
{
	t_shell	*shell;
	char	*input;
	int		exit_status;

	if (argc > 1 && argv[1])
		return (printf("Invalid number of arguments"), 1);
	shell = malloc(sizeof(t_shell));
	if (!shell)
		malloc_fail(NULL);
	init_shell(shell, env);
	while (1)
	{
		setup_signals(shell);
		input = readline("minishell$ ");
		add_history(input);
		add_to_garbage(shell, input);
		if (!input || input[0] == EOF)
			break ;
		if (parse(shell, input) != -1)
			execution(shell, shell->execution_list);
		reset_shell(shell);
		gc_free(shell, input);
	}
	exit_status = g_exit_code;
	return (free_garbage(shell->garbage), exit_status);
}
