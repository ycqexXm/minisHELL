/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 19:33:31 by yorlians          #+#    #+#             */
/*   Updated: 2023/07/31 03:46:26 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_export(t_shell *shell, char **export)
{
	int		i;
	char	*value;
	char	*variable;

	i = 0;
	while (export && export[i])
	{
		variable = export_variable(shell, export[i]);
		value = export_value(shell, export[i]);
		printf("declare -x %s", variable);
		if (value)
		{
			printf("\"%s\"", value);
			gc_free(shell, value);
		}
		printf("\n");
		gc_free(shell, variable);
		i++;
	}
}

int	find_variable(t_shell *shell, char **export, char *command)
{
	int		i;
	char	*variable;
	int		var_length;

	i = 0;
	if (find_char(command, '=') == NULL)
		variable = string_duplicate(shell, command);
	else
		variable = str_n_dup(shell, command, find_char(command, '=') - command);
	var_length = str_len(variable);
	while (export && export[i])
	{
		if (string_n_compare(variable, export[i], var_length) == 0 \
			&& (!export[i][var_length] || export[i][var_length] == '='))
			return (gc_free(shell, variable), i);
		i++;
	}
	return (gc_free(shell, variable), -1);
}

int	update_variable(t_shell *shell, char *command)
{
	char	**env;
	char	**export;
	int		variable_position;

	export = shell->export;
	env = shell->env;
	if (find_char(command, '=') == NULL)
		return (EXIT_SUCCESS);
	variable_position = find_variable(shell, export, command);
	gc_free(shell, export[variable_position]);
	export[variable_position] = string_duplicate(shell, command);
	variable_position = find_variable(shell, env, command);
	if (variable_position != -1)
	{
		gc_free(shell, env[variable_position]);
		env[variable_position] = string_duplicate(shell, command);
	}
	else
		add_variable(shell, &shell->env, command);
	return (0);
}

int	add_variable(t_shell *shell, char ***list_ptr, char *command)
{
	int		i;
	char	**old_list;
	char	**new_list;

	i = 0;
	old_list = *list_ptr;
	while (old_list[i])
		i++;
	new_list = gc_malloc(shell, (i + 2) * sizeof(char *));
	i = 0;
	while (old_list[i])
	{
		new_list[i] = old_list[i];
		i++;
	}
	new_list[i] = string_duplicate(shell, command);
	i++;
	new_list[i] = NULL;
	gc_free(shell, old_list);
	*list_ptr = new_list;
	return (0);
}

int	export(t_shell *shell, t_token *command)
{
	int	status;

	status = 0;
	command = command->next;
	if (!command)
		print_export(shell, shell->export);
	while (command)
	{
		if (check_export_syntax(command->token))
		{
			if (find_variable(shell, shell->export, command->token) != -1)
				update_variable(shell, command->token);
			else
			{
				if (find_char(command->token, '=') != NULL)
					add_variable(shell, &shell->env, command->token);
				add_variable(shell, &shell->export, command->token);
			}
			status = 0;
		}
		else
			status = 1 + (*command->token == '-');
		command = command->next;
	}
	return (status);
}
