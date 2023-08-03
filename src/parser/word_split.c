/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 18:31:05 by bluzi             #+#    #+#             */
/*   Updated: 2023/07/31 00:33:12 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_splittable(char *str)
{
	int	i;

	i = 0;
	while (str[i] && !is_space(str[i]))
		i += skip_quotes(str + i) + 1;
	return (is_space(str[i]));
}

void	split_token(t_shell *shell, t_execution *block, char *token, int id)
{
	int		i;
	int		y;
	char	*new_token;

	i = skip_spaces(token);
	y = i;
	while (token && token[i])
	{
		while (token[i] && !is_space(token[i]))
			i += skip_quotes(token + i) + 1;
		new_token = str_n_dup(shell, token + y, i - y);
		if (id < 4 || id == REDIR_FILE || id == DELIMITER)
			list_append(shell, &block->redir, new_token, id);
		else
			list_append(shell, &block->command, new_token, id);
		while (is_space(token[i]))
			i++;
		y = i;
	}
}

t_token	*ambiguos_redir(t_execution *block, t_token *token_list)
{
	block->redir = NULL;
	block->command = NULL;
	printf("%s\t'%s'\n", SYNTAX_ERR, "ambiguos redirect");
	while (token_list->next && token_list->next->id != PIPE)
		token_list = token_list->next;
	return (token_list);
}

void	word_split(t_shell *shell, t_token *token_list)
{
	t_execution	*block;

	block = execution_create(shell);
	while (token_list)
	{
		if (token_list->id == PIPE)
		{
			execution_append(&shell->execution_list, block);
			shell->pipes_count++;
			block = execution_create(shell);
		}
		else
		{
			if (is_splittable(token_list->token)
				&& token_list->id == REDIR_FILE)
				token_list = ambiguos_redir(block, token_list);
			else
				split_token(shell, block, token_list->token, token_list->id);
		}
		token_list = token_list->next;
	}
	execution_append(&shell->execution_list, block);
}
