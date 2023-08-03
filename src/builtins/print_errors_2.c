/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_errors_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 03:35:22 by yorlians          #+#    #+#             */
/*   Updated: 2023/07/31 03:35:39 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_permission_error(char *directory)
{
	write(2, "minishell: cd: ", str_len("minishell: cd: "));
	write(2, directory, str_len(directory));
	write(2, ": Permission denied\n", str_len(": Permission denied\n"));
}

void	print_file_error(char *directory)
{
	write(2, "minishell: cd: ", str_len("minishell: cd: "));
	write(2, directory, str_len(directory));
	write(2, ": No such file or directory\n",
		str_len(": No such file or directory\n"));
}
