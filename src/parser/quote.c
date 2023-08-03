/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bluzi <bluzi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 17:20:34 by bluzi             #+#    #+#             */
/*   Updated: 2023/07/30 19:38:08 by bluzi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	remove_quote_length(char *copy)
{
	int	i;

	i = 0;
	if (skip_quotes(copy))
		i = skip_quotes(copy);
	else
		while (copy[i] && !skip_quotes(copy + i))
			i++;
	return (i);
}

void	remove_quotes(t_shell *shell, char **token_ptr, char *copy)
{
	int		i;
	char	*unquoted_string;
	char	*to_free;
	char	*to_join;

	i = 0;
	unquoted_string = NULL;
	while (copy && copy[i])
	{
		i = remove_quote_length(copy);
		if (skip_quotes(copy))
			to_join = str_n_dup(shell, copy + 1, i++ - 1);
		else
			to_join = str_n_dup(shell, copy, i);
		to_free = unquoted_string;
		unquoted_string = string_join(shell, unquoted_string, to_join);
		copy += i;
		i = 0;
		if (to_join)
			gc_free(shell, to_join);
		if (to_free)
			gc_free(shell, to_free);
	}
	*token_ptr = unquoted_string;
}

void	unquote(t_shell *shell, t_execution *block)
{
	t_token	*redir;
	t_token	*command;

	while (block)
	{
		redir = block->redir;
		while (redir)
		{
			remove_quotes(shell, &redir->token, redir->token);
			redir = redir->next;
		}
		command = block->command;
		while (command)
		{
			remove_quotes(shell, &command->token, command->token);
			command = command->next;
		}
		block = block->next;
	}
}

int	skip_single_quotes(char *input)
{
	int	i;

	i = 0;
	if (input[i] == '\'')
	{
		i++;
		while (input[i] && input[i] != '\'')
			i++;
	}
	if (input[i] == '\0')
		return (0);
	else
		return (i);
}

int	skip_quotes(char *input)
{
	int	i;

	i = 0;
	if (input[i] == '\"')
	{
		i++;
		while (input[i] && input[i] != '\"')
			i++;
	}
	else if (input[i] == '\'')
	{
		i++;
		while (input[i] && input[i] != '\'')
			i++;
	}
	if (input[i] == '\0')
		return (0);
	else
		return (i);
}
