/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:53:24 by bluzi             #+#    #+#             */
/*   Updated: 2023/07/31 16:23:02 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(char *error, char *token)
{
	write(2, "minishell: ", str_len("minishell: "));
	write(2, error, str_len(error));
	write(2, " `", str_len(" `"));
	write(2, token, str_len(token));
	write(2, "\'\n", str_len("\'\n"));
}

int	check_syntax(t_token *last_token, char *token)
{
	if (last_token == NULL && string_compare("|", token) == 0)
		return (print_error(SYNTAX_ERR, token), -1);
	else if (last_token == NULL)
		return (0);
	else if (last_token->id < 4 && string_compare(token, ">") == 0)
		return (print_error(SYNTAX_ERR, token), -1);
	else if (last_token->id < 4 && string_compare(token, ">>") == 0)
		return (print_error(SYNTAX_ERR, token), -1);
	else if (last_token->id < 4 && string_compare(token, "<<") == 0)
		return (print_error(SYNTAX_ERR, token), -1);
	else if (last_token->id < 4 && string_compare(token, "<") == 0)
		return (print_error(SYNTAX_ERR, token), -1);
	else if (last_token->id < 5 && string_compare(token, "|") == 0)
		return (print_error(SYNTAX_ERR, token), -1);
	return (0);
}

void	add_token(t_shell *shell, t_token **token_list, char *token)
{
	int	pre_id;

	if (string_compare(token, "<<") == 0)
		list_append(shell, token_list, token, HERE_DOC);
	else if (string_compare(token, "<") == 0)
		list_append(shell, token_list, token, IN_REDIR);
	else if (string_compare(token, ">") == 0)
		list_append(shell, token_list, token, OUT_REDIR);
	else if (string_compare(token, ">>") == 0)
		list_append(shell, token_list, token, APPEND);
	else if (string_compare(token, "|") == 0)
		list_append(shell, token_list, token, PIPE);
	else
	{
		pre_id = -1;
		if (*token_list)
			pre_id = last_token(*token_list)->id;
		if (pre_id == HERE_DOC)
			handle_heredoc(shell, token_list, token);
		else if (pre_id == IN_REDIR || pre_id == OUT_REDIR || pre_id == APPEND)
			list_append(shell, token_list, token, REDIR_FILE);
		else
			list_append(shell, token_list, token, WORD);
	}
}

int	tokenizer(t_shell *shell, t_token **token_list, char *input)
{
	int		i;
	int		y;
	char	*token;

	i = skip_spaces(input);
	y = i;
	while (input && input[i])
	{
		if (is_separator(input + i))
			i += is_separator(input + i);
		else
			while (input[i] && !is_separator(input + i) && !is_space(input[i]))
				i += skip_quotes(input + i) + 1;
		token = str_n_dup(shell, input + y, i - y);
		if (check_syntax(last_token(*token_list), token) == -1)
			return (g_exit_code = 2, gc_free(shell, token), -1);
		add_token(shell, &shell->token_list, token);
		while (is_space(input[i]))
			i++;
		y = i;
	}
	if ((*token_list) && last_token(*token_list)->id < 5)
		return (g_exit_code = 2, print_error(SYNTAX_ERR, "newline"), -1);
	return (0);
}
