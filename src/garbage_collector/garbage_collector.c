/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 17:10:52 by bluzi             #+#    #+#             */
/*   Updated: 2023/07/31 03:32:05 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_garbage(t_trash *garbage)
{
	t_trash	*to_free;
	int		i;

	i = 3;
	while (garbage)
	{
		to_free = garbage;
		garbage = garbage -> next;
		free(to_free -> ptr);
		free(to_free);
	}
	while (i < 1024)
	{
		if (isatty(i))
			close(i);
		i++;
	}
}

void	malloc_fail(t_shell *shell)
{
	if (shell)
		free_garbage(shell->garbage);
	write(2, "error : malloc failed\n", 23);
	exit(1);
}

void	add_to_garbage(t_shell *shell, void *ptr)
{
	t_trash	*temp;
	t_trash	**garbage;

	garbage = &shell->garbage;
	if (ptr == NULL)
		return ;
	temp = malloc(sizeof(t_trash));
	if (temp == NULL)
	{
		free(ptr);
		malloc_fail(shell);
	}
	temp -> next = NULL;
	temp -> ptr = ptr;
	if (*garbage == NULL)
		*garbage = temp;
	else
	{
		temp->next = *garbage;
		*garbage = temp;
	}
}

void	*gc_malloc(t_shell *shell, size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (ptr == NULL)
		malloc_fail(shell);
	add_to_garbage(shell, ptr);
	return (ptr);
}

void	gc_free(t_shell *shell, void *ptr)
{
	t_trash	**garbage_ptr;
	t_trash	*garbage;
	t_trash	*previous;

	garbage_ptr = &shell->garbage;
	garbage = *garbage_ptr;
	previous = NULL;
	while (garbage)
	{
		if (garbage -> ptr == ptr)
			break ;
		previous = garbage;
		garbage = garbage -> next;
	}
	if (!garbage || garbage -> ptr != ptr)
	{
		free_garbage(shell->garbage);
		exit(1);
	}
	if (previous == NULL)
		*garbage_ptr = garbage -> next;
	else
		previous -> next = garbage -> next;
	free(garbage -> ptr);
	free(garbage);
}
