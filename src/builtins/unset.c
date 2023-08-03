/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 20:24:09 by yorlians          #+#    #+#             */
/*   Updated: 2023/07/31 01:42:47 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_unset_syntax(char *token)
{
	int	i;

	i = 0;
	if (*token == '-')
		return (print_invalid_option_un(token), 0);
	if (is_alpha(*token) || *token == '_')
		i++;
	else
		return (print_invalid_identifier_un(token), 0);
	while (token && token[i])
	{
		if (!is_alpha_num(token[i]) && token[i] != '_')
			return (print_invalid_identifier_un(token), 0);
		i++;
	}
	return (1);
}

int	remove_variable(t_shell *shell, char ***list_ptr, char *command)
{
	char	**list;
	int		variable_position;
	int		i;
	int		j;

	i = 0;
	list = *list_ptr;
	while (list[i])
		i++;
	variable_position = find_variable(shell, list, command);
	list = gc_malloc(shell, i * sizeof(char *));
	i = 0;
	j = 0;
	while ((*list_ptr)[j])
	{
		if (j == variable_position)
			gc_free(shell, (*list_ptr)[j++]);
		else
			list[i++] = (*list_ptr)[j++];
	}
	list[i] = NULL;
	gc_free(shell, *list_ptr);
	*list_ptr = list;
	return (EXIT_SUCCESS);
}

int	unset(t_shell *shell, t_token *command)
{
	int	status;

	status = 0;
	command = command->next;
	if (!command)
		return (EXIT_SUCCESS);
	while (command)
	{
		if (check_unset_syntax(command->token))
		{
			if (find_variable(shell, shell->export, command->token) != -1)
				remove_variable(shell, &shell->export, command->token);
			if (find_variable(shell, shell->env, command->token) != -1)
				remove_variable(shell, &shell->env, command->token);
			status = 0;
		}
		else
			status = 1 + (*command->token == '-');
		command = command->next;
	}
	return (status);
}
