/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:21:02 by bluzi             #+#    #+#             */
/*   Updated: 2023/07/31 00:33:24 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse(t_shell *shell, char *input)
{
	if (tokenizer(shell, &shell->token_list, input) == -1)
		return (-1);
	expand(shell, shell->token_list);
	word_split(shell, shell->token_list);
	unquote(shell, shell->execution_list);
	return (0);
}
