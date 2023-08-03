/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 14:58:48 by bluzi             #+#    #+#             */
/*   Updated: 2023/07/31 01:58:51 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_output_redir(t_token *redir, int *std_fd, int *redir_fd)
{
	*std_fd = STDOUT_FILENO;
	if (redir->id == OUT_REDIR)
		*redir_fd = open(redir->next->token, \
			O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else
		*redir_fd = open(redir->next->token, \
			O_WRONLY | O_APPEND | O_CREAT, 0644);
}

void	redirs(t_shell *shell, t_token *redir)
{
	int	std_fd;
	int	redir_fd;

	while (redir)
	{
		if (redir->id == IN_REDIR || redir->id == HERE_DOC)
		{
			std_fd = STDIN_FILENO;
			redir_fd = open(redir->next->token, O_RDONLY);
		}
		else if (redir->id == OUT_REDIR || redir->id == APPEND)
			handle_output_redir(redir, &std_fd, &redir_fd);
		if (redir_fd < 0)
		{
			dup2(2, 1);
			printf("minishell: %s%s", redir->next->token, FILE_DIR_ERR);
			free_garbage(shell->garbage);
			exit(1);
		}
		dup2(redir_fd, std_fd);
		close(redir_fd);
		redir = redir->next->next;
	}
}

void	piping(t_shell *shell, int n)
{
	int	fd_input;
	int	fd_output;

	if (n)
	{
		fd_input = shell->pipes[n - 1][READ_END];
		dup2(fd_input, STDIN_FILENO);
		close(fd_input);
	}
	else if (shell->pipes_count)
		close(shell->pipes[0][READ_END]);
	if (n < shell->pipes_count)
	{
		fd_output = shell -> pipes[n][WRITE_END];
		dup2(fd_output, STDOUT_FILENO);
		close(fd_output);
	}
	while (--n >= 0)
	{
		close(shell -> pipes[n][READ_END]);
		close(shell -> pipes[n][WRITE_END]);
	}
}
