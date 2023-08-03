/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 00:02:56 by yorlians          #+#    #+#             */
/*   Updated: 2023/07/31 03:44:38 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_export_syntax(char *token)
{
	int	i;

	i = 0;
	if (*token == '-')
		return (print_invalid_option_ex(token), 0);
	if (is_alpha(*token) || *token == '_')
		i++;
	else
		return (print_invalid_identifier_ex(token), 0);
	while (token && token[i])
	{
		if (token[i] == '=')
			break ;
		else if (!is_alpha_num(token[i]) && token[i] != '_')
			return (print_invalid_identifier_ex(token), 0);
		i++;
	}
	return (1);
}

char	*export_variable(t_shell *shell, char *export)
{
	char	*variable;

	if (find_char(export, '='))
		variable = str_n_dup(shell, export,
				find_char(export, '=') - export + 1);
	else
		variable = string_duplicate(shell, export);
	return (variable);
}

char	*export_value(t_shell *shell, char *export)
{
	char	*value;

	if (find_char(export, '='))
		value = string_duplicate(shell, find_char(export, '=') + 1);
	else
		value = NULL;
	return (value);
}
