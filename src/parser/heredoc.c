/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 15:37:31 by bluzi             #+#    #+#             */
/*   Updated: 2023/07/31 06:29:57 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	destroy_heredocs(t_shell *shell)
{
	char	*heredoc_file;
	char	*heredoc_num;

	while (shell->heredoc_count >= 0)
	{
		heredoc_num = itoa(shell, shell->heredoc_count--);
		heredoc_file = string_join(shell, "heredoc_", heredoc_num);
		gc_free(shell, heredoc_num);
		unlink(heredoc_file);
		gc_free(shell, heredoc_file);
	}
}

void	handle_heredoc(t_shell *shell, t_token **token_list, char *token)
{
	char	*heredoc_file;
	char	*delimiter;
	char	*unquoted_delimiter;

	delimiter = string_join(shell, token, "\n");
	remove_quotes(shell, &unquoted_delimiter, delimiter);
	if (string_compare(unquoted_delimiter, delimiter) == 0)
		heredoc_file = create_heredoc(shell, delimiter, 1);
	else
		heredoc_file = create_heredoc(shell, unquoted_delimiter, 0);
	gc_free(shell, delimiter);
	gc_free(shell, unquoted_delimiter);
	gc_free(shell, token);
	list_append(shell, token_list, heredoc_file, REDIR_FILE);
}

char	*create_heredoc(t_shell *shell, char *delimiter, int expand)
{
	int		fd;
	char	*line;
	char	*heredoc_file;
	char	*heredoc_num;

	heredoc_num = itoa(shell, ++shell->heredoc_count);
	heredoc_file = string_join(shell, "heredoc_", heredoc_num);
	gc_free(shell, heredoc_num);
	fd = open(heredoc_file, O_RDWR | O_TRUNC | O_CREAT, 0644);
	while (1)
	{
		write(0, "> ", str_len("> "));
		line = get_next_line(0);
		add_to_garbage(shell, line);
		if (!line || *line == EOF || !string_compare(line, delimiter))
			break ;
		if (expand)
			expand_string(shell, &line, line, 1);
		write(fd, line, str_len(line));
		gc_free(shell, line);
	}
	if (line)
		gc_free(shell, line);
	close(fd);
	return (heredoc_file);
}
