/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:57:19 by bluzi             #+#    #+#             */
/*   Updated: 2023/07/30 00:47:23 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_exec(t_execution *list)
{
	int	i;

	i = 0;
	while (list)
	{
		printf("BLOCK %d\n", i++);
		printf("\tRedirections\n");
		while (list->redir)
		{
			printf("\t\t%s\n", list->redir->token);
			list->redir = list->redir->next;
		}
		printf("\n\tWords\n");
		while (list->command)
		{
			printf("\t\t%s\n", list->command->token);
			list->command = list->command->next;
		}
		list = list -> next;
	}
}

void	print_list(t_token *list)
{
	while (list)
	{
		printf("[%s] id -> %d\n", list->token, list->id);
		list = list -> next;
	}
}
