/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 00:01:41 by yorlians          #+#    #+#             */
/*   Updated: 2023/07/31 04:12:28 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_invalid_option_un(char *token)
{
	write(2, "minishell: unset: ", 19);
	write(2, token, str_len(token));
	write(2, ": invalid option\n", 18);
}

void	print_invalid_identifier_un(char *token)
{
	write(2, "minishell: unset: ", 19);
	write(2, token, str_len(token));
	write(2, ": not a valid identifier\n", 26);
}

void	print_invalid_option_ex(char *token)
{
	write(2, "minishell: export: ", 20);
	write(2, token, str_len(token));
	write(2, ": invalid option\n", 18);
}

void	print_invalid_identifier_ex(char *token)
{
	write(2, "minishell: export: ", 20);
	write(2, token, str_len(token));
	write(2, ": not a valid identifier\n", 26);
}
