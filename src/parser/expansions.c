/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 16:28:05 by bluzi             #+#    #+#             */
/*   Updated: 2023/07/31 16:23:02 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_expandable(char *str, int is_in_quote)
{
	int	i;

	i = 0;
	if (str && str[i] == '$' && (is_alpha(str[i + 1]) || str[i + 1] == '_'
			|| str[i + 1] == '?' || ((str[i + 1] == '\'' || str[i + 1] == '\"')
				&& !is_in_quote)))
	{
		i++;
		if (str[i] == '?')
			return (++i);
		if (skip_quotes(str + i))
			return (i + skip_quotes(str + i) + 1);
		else if (str[i] == '\'' || (str[i] == '\"' && !skip_quotes(str + i)))
			return (0);
		while (str[i] && (is_alpha_num(str[i]) || str[i] == '_'))
			i++;
	}
	return (i);
}

char	*expand_variable(t_shell *shell, char *var, char **env)
{
	char	*empty;
	char	*new_var;
	int		var_length;

	new_var = str_n_dup(shell, var + 1, find_expandable(var, 0) - 1);
	if (string_compare(new_var, "?") == 0)
		return (gc_free(shell, new_var), itoa(shell, g_exit_code));
	if (skip_quotes(new_var))
		return (new_var);
	var_length = str_len(new_var);
	while (env && *env)
	{
		if (string_n_compare(*env, new_var, var_length) == 0 \
			&& *(*env + var_length) == '=')
		{
			gc_free(shell, new_var);
			return (string_duplicate(shell, *env + var_length + 1));
		}
		else
			env++;
	}
	empty = gc_malloc(shell, 1);
	*empty = '\0';
	return (empty);
}

int	find_expandable(char *copy, int heredoc)
{
	int	i;
	int	is_in_quote;

	i = 0;
	is_in_quote = heredoc;
	if (is_expandable(copy, is_in_quote))
		return (is_expandable(copy, is_in_quote));
	while (copy[i] && !is_expandable(copy + i, is_in_quote))
	{
		if (copy[i] == '\"' && !heredoc)
			is_in_quote = !is_in_quote;
		i += !is_in_quote * skip_single_quotes(copy + i) + 1;
	}
	return (i);
}

void	expand_string(t_shell *shell, char **token_ptr, char *copy, int heredoc)
{
	int		i;
	char	*expanded_string;
	char	*to_free;
	char	*to_join;

	i = 0;
	expanded_string = NULL;
	while (copy && copy[i])
	{
		if (is_expandable(copy, 0))
			to_join = expand_variable(shell, copy, shell->env);
		else
			to_join = str_n_dup(shell, copy, find_expandable(copy, heredoc));
		i += find_expandable(copy, heredoc);
		to_free = expanded_string;
		expanded_string = string_join(shell, expanded_string, to_join);
		copy += i;
		i = 0;
		if (to_join)
			gc_free(shell, to_join);
		if (to_free)
			gc_free(shell, to_free);
	}
	gc_free(shell, *token_ptr);
	*token_ptr = expanded_string;
}

void	expand(t_shell *shell, t_token *list)
{
	while (list)
	{
		if (list->id != DELIMITER)
			expand_string(shell, &list->token, list->token, 0);
		list = list->next;
	}
}
