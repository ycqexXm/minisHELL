/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bluzi <bluzi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 15:39:34 by bluzi             #+#    #+#             */
/*   Updated: 2023/07/30 20:05:01 by bluzi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_size_num(int n)
{
	int	q;

	q = 1;
	if (n < 0)
		q++;
	while (n < -9 || n > 9)
	{
		q++;
		n = n / 10;
	}
	return (q);
}

void	ft_fillnum(char *num, int n, int i, int q)
{
	while (q >= i)
	{	
		num[q--] = (n % 10) + '0';
		n = n / 10;
	}
}

char	*itoa(t_shell *shell, int n)
{
	char	*num;
	int		q;
	int		i;

	i = 0;
	q = ft_size_num(n);
	num = gc_malloc(shell, (q + 1) * sizeof(char));
	num[q--] = '\0';
	if (n < 0)
	{
		num[i++] = '-';
		if (n == -2147483648)
		{	
			num[i++] = '2';
			n = -147483648;
		}
		n = -n;
	}
	ft_fillnum(num, n, i, q);
	return (num);
}
