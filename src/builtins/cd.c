/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 19:40:01 by yorlians          #+#    #+#             */
/*   Updated: 2023/07/31 06:14:01 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_enviroment(t_shell *shell, char *old_directory, char *directory)
{
	char	*temp;

	if (!directory)
		malloc_fail(shell);
	add_to_garbage(shell, directory);
	temp = string_join(shell, "OLDPWD=", old_directory);
	if (find_variable(shell, shell->export, temp) != -1)
		update_variable(shell, temp);
	else
	{
		if (find_char(temp, '=') != NULL)
			add_variable(shell, &shell->env, temp);
		add_variable(shell, &shell->export, temp);
	}
	gc_free(shell, temp);
	temp = string_join(shell, "PWD=", directory);
	if (find_variable(shell, shell->export, temp) != -1)
		update_variable(shell, temp);
	else
	{
		if (find_char(temp, '=') != NULL)
			add_variable(shell, &shell->env, temp);
		add_variable(shell, &shell->export, temp);
	}
	gc_free(shell, temp);
}

char	*home_dir(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (string_n_compare(env[i], "HOME=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

int	cd(t_shell *shell, char **arguments, char **env)
{
	char	*directory;
	char	*old_directory;

	old_directory = getcwd(NULL, 0);
	add_to_garbage(shell, old_directory);
	if (!arguments[1])
		directory = string_duplicate(shell, home_dir(env));
	else
		directory = string_duplicate(shell, arguments[1]);
	if (access(directory, F_OK) == 0 && is_directory(directory))
	{
		if (!access(directory, X_OK))
			chdir(directory);
		else
			return (print_permission_error(directory), 1);
		change_enviroment(shell, old_directory, getcwd(NULL, 0));
	}
	else
		return (print_file_error(directory), 1);
	gc_free(shell, directory);
	gc_free(shell, old_directory);
	return (EXIT_SUCCESS);
}
