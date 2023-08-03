/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 00:29:27 by yorlians          #+#    #+#             */
/*   Updated: 2023/07/31 00:29:35 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_duoble_ptr(t_shell *shell, char **ptr)
{
	int	i;

	i = 0;
	while (ptr && ptr[i])
		gc_free(shell, ptr[i++]);
	gc_free(shell, ptr);
}

void	free_token_list(t_shell *shell, t_token *list)
{
	t_token	*to_free;

	while (list)
	{
		to_free = list;
		list = list->next;
		if (to_free->token)
			gc_free(shell, to_free->token);
		gc_free(shell, to_free);
	}
}

void	free_execution_list(t_shell *shell, t_execution *list)
{
	t_execution	*to_free;

	while (list)
	{
		to_free = list;
		list = list->next;
		if (to_free->redir)
			free_token_list(shell, to_free->redir);
		if (to_free->command)
			free_token_list(shell, to_free->command);
		gc_free(shell, to_free);
	}
}

void	free_shell(t_shell *shell)
{
	if (shell->env)
		free_duoble_ptr(shell, shell->env);
	if (shell->export)
		free_duoble_ptr(shell, shell->export);
	if (shell->token_list)
		free_token_list(shell, shell->token_list);
	if (shell->execution_list)
		free_execution_list(shell, shell->execution_list);
	gc_free(shell, shell);
}
