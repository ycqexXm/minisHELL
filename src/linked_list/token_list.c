/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bluzi <bluzi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:56:33 by bluzi             #+#    #+#             */
/*   Updated: 2023/07/30 20:04:46 by bluzi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	list_append(t_shell *shell, t_token **list, char *token, int id)
{
	t_token	*temp;
	t_token	*node;

	if (!list)
		return ;
	node = gc_malloc(shell, sizeof(t_token));
	node->token = token;
	node->id = id;
	node->next = NULL;
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

t_token	*last_token(t_token *list)
{
	if (!list)
		return (NULL);
	while (list -> next)
		list = list->next;
	return (list);
}

int	list_length(t_token *list)
{
	int	i;

	i = 0;
	while (list)
	{
		list = list->next;
		i++;
	}
	return (i);
}

char	**list_to_array(t_shell *shell, t_token *list)
{
	int		i;
	char	**command;

	i = 0;
	command = gc_malloc(shell, (list_length(list) + 1) * sizeof(char *));
	while (list)
	{
		command[i++] = string_duplicate(shell, list->token);
		list = list -> next;
	}
	command[i] = NULL;
	return (command);
}
