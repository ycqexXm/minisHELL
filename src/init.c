/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:11:58 by bluzi             #+#    #+#             */
/*   Updated: 2023/07/31 16:22:22 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_paths(t_shell *shell, char **env)
{
	char	*to_free;
	int		i;

	i = -1;
	shell->paths = NULL;
	while (env && env[++i])
		if (!string_n_compare(env[i], "PATH=", 5))
			shell->paths = split(shell, env[i] + 5, ':');
	if (!env || !(*env) || !shell->paths)
	{
		shell->paths = gc_malloc(shell, 2 * sizeof(char *));
		shell->paths[0] = getcwd(NULL, 0);
		add_to_garbage(shell, shell->paths[0]);
		shell->paths[1] = NULL;
	}
	i = -1;
	while (shell->paths[++i])
	{
		to_free = shell->paths[i];
		shell->paths[i] = string_join(shell, shell->paths[i], "/");
		gc_free(shell, to_free);
	}
}

void	copy_env(t_shell *shell, char **env)
{
	int	i;
	int	j;
	int	lines;

	i = -1;
	j = 0;
	lines = 0;
	shell->env = NULL;
	while (env[++i])
		lines += (string_n_compare(env[i], "OLDPWD=", 7) != 0);
	i = 0;
	shell->env = gc_malloc(shell, (lines + 1) * sizeof(char *));
	shell->export = gc_malloc(shell, (lines + 1) * sizeof(char *));
	while (env[j])
	{
		if (string_n_compare(env[j], "OLDPWD=", 7) != 0)
		{
			shell->env[i] = string_duplicate(shell, env[j]);
			shell->export[i] = string_duplicate(shell, env[j]);
			i++;
		}
		j++;
	}
	shell->env[i] = NULL;
	shell->export[i] = NULL;
}

void	init_shell(t_shell *shell, char **env)
{
	shell->garbage = NULL;
	add_to_garbage(shell, shell);
	shell->execution_list = NULL;
	shell->token_list = NULL;
	shell->env = NULL;
	shell->export = NULL;
	shell->pipes = NULL;
	shell->pids = NULL;
	shell->paths = NULL;
	shell->heredoc_count = 0;
	shell->pipes_count = 0;
	copy_env(shell, env);
}
