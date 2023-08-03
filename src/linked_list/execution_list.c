/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bluzi <bluzi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:56:33 by bluzi             #+#    #+#             */
/*   Updated: 2023/07/30 20:04:36 by bluzi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_execution	*execution_create(t_shell *shell)
{
	t_execution	*node;

	node = gc_malloc(shell, sizeof(t_execution));
	node->redir = NULL;
	node->command = NULL;
	node->next = NULL;
	return (node);
}

void	execution_append(t_execution **list, t_execution *node)
{
	t_execution	*temp;

	if (!list)
		return ;
	temp = *list;
	if (!temp)
		*list = node;
	else
	{
		while (temp -> next)
			temp = temp->next;
		temp->next = node;
	}
}

t_execution	*last_execution(t_execution *list)
{
	if (!list)
		return (NULL);
	while (list -> next)
		list = list->next;
	return (list);
}
