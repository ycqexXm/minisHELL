/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 05:37:19 by yorlians          #+#    #+#             */
/*   Updated: 2023/07/31 06:59:35 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	termios_setup(t_shell *shell)
{
	tcgetattr(STDOUT_FILENO, &shell->termios);
	shell->termios.c_lflag &= ~ECHOCTL;
	tcsetattr(STDOUT_FILENO, TCSAFLUSH, &shell->termios);
}

void	handle_sig_int(int signum)
{
	if (signum == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit_code = 1;
		return ;
	}
}

void	setup_signals(t_shell *shell)
{
	struct sigaction	sig_int;

	termios_setup(shell);
	sig_int.sa_handler = &handle_sig_int;
	sigemptyset(&sig_int.sa_mask);
	sig_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sig_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}
