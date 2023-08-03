/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorlians <yorlians@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 19:59:53 by bluzi             #+#    #+#             */
/*   Updated: 2023/07/31 16:25:02 by yorlians         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/******************************************************************************
*								  Libraries									  *
******************************************************************************/

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <signal.h>

/******************************************************************************
*								  Macros									  *
******************************************************************************/

# define FILE_DIR_ERR		": No such file or directory\n"
# define SYNTAX_ERR			"minishell: syntax error near unexpected token"
# define COMMAND_ERR		"command not found\n"
# define NO_ACCESS			"Permission denied\n"
# define READ_END			0
# define WRITE_END			1

extern int	g_exit_code;

/******************************************************************************
*								  Structures								  *
******************************************************************************/

typedef enum e_id
{
	OUT_REDIR,
	IN_REDIR,
	APPEND,
	HERE_DOC,
	PIPE,
	WORD,
	REDIR_FILE,
	DELIMITER,
	BUILTIN
}				t_id;

typedef enum e_builtin
{
	ECO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT
}				t_builtin;

typedef struct s_trash
{
	void				*ptr;
	struct s_trash		*next;
}				t_trash;

typedef struct s_token
{
	char				*token;
	int					id;
	struct s_token		*next;
}				t_token;

typedef struct s_execution
{
	t_token				*redir;
	t_token				*command;
	struct s_execution	*next;
}				t_execution;

typedef struct s_shell
{
	struct termios		termios;
	int					heredoc_count;
	int					pipes_count;
	int					**pipes;
	pid_t				*pids;
	char				**env;
	char				**paths;
	char				**export;
	t_trash				*garbage;
	t_token				*token_list;
	t_execution			*execution_list;
}				t_shell;

/******************************************************************************
*								  Functions									  *
******************************************************************************/

/*~~~~~~~~~~~~ BUILTINS ~~~~~~~~~~~~~~*/

//			cd
void		change_enviroment(t_shell *shell, char *old_directory,
				char *directory);
char		*home_dir(char **env);
int			cd(t_shell *shell, char **arguments, char **env);
//			echo
int			option_n_check(char	*string);
int			echo(t_shell *shell, char **command);
//			env
int			env(char **arguments, char **env);
//			exit
void		check_exit_syntax(t_shell *shell, char *arguments);
int			shell_exit(t_shell *shell, char **arguments);
//			export
int			check_export_syntax(char *token);
char		*export_variable(t_shell *shell, char *export);
char		*export_value(t_shell *shell, char *export);
void		print_export(t_shell *shell, char **export);
int			find_variable(t_shell *shell, char **export, char *command);
int			update_variable(t_shell *shell, char *command);
int			add_variable(t_shell *shell, char ***list_ptr, char *command);
int			export(t_shell *shell, t_token *command);
//			pwd
int			pwd(void);
//			unset
int			check_unset_syntax(char *token);
int			remove_variable(t_shell *shell, char ***list_ptr, char *command);
int			unset(t_shell *shell, t_token *command);
//			print_errors
void		print_invalid_option_un(char *token);
void		print_invalid_identifier_un(char *token);
void		print_invalid_option_ex(char *token);
void		print_invalid_identifier_ex(char *token);
void		print_permission_error(char *directory);
void		print_file_error(char *directory);
//			other
int			is_builtin(t_shell *shell, t_token *node);
int			execute_builtin(t_shell *shell, t_token	*redir, t_token *command);
void		restore_std_fd(int *std_fd);

/*~~~~~~~~~~~~ EXECUTION ~~~~~~~~~~~~~~*/

//			command
int			is_directory(char *path);
void		print_is_dir(char *command);
int			exist_and_exec(t_shell *shell, char *command);
void		print_command_err(t_shell *shell, char *command,
				char *error_message);
char		*find_executable(t_shell *shell, char *command, char **paths);
//			execution
void		child_process(t_shell *shell, t_execution *block, int n);
void		close_and_wait(t_shell *shell);
void		allocate_pipes_and_pids(t_shell *shell);
void		execution(t_shell *shell, t_execution *execution_list);
//			redirections
void		handle_output_redir(t_token *redir, int *std_fd, int *redir_fd);
void		redirs(t_shell *shell, t_token *redir);
void		piping(t_shell *shell, int n);

/*~~~~~~~~~~~~ GARBAGE COLLECTOR ~~~~~~~~~~~~~~*/

void		free_garbage(t_trash *garbage);
void		malloc_fail(t_shell *shell);
void		add_to_garbage(t_shell *shell, void *ptr);
void		*gc_malloc(t_shell *shell, size_t size);
void		gc_free(t_shell *shell, void *ptr);

/*~~~~~~~~~~~~ GET NEXT LINE ~~~~~~~~~~~~~~*/
char		*get_next_line(int fd);

/*~~~~~~~~~~~~ LINKED LIST ~~~~~~~~~~~~~~*/

//			execution list
t_execution	*execution_create(t_shell *shell);
void		execution_append(t_execution **list, t_execution *node);
t_execution	*last_execution(t_execution *list);
//			token list
void		list_append(t_shell *shell, t_token **list, char *token, int id);
t_token		*last_token(t_token *list);
int			list_length(t_token *list);
char		**list_to_array(t_shell *shell, t_token *list);

/*~~~~~~~~~~~~ PARSER ~~~~~~~~~~~~~~*/

//			expansions
int			is_expandable(char *str, int is_in_quote);
char		*expand_variable(t_shell *shell, char *var, char **env);
int			find_expandable(char *copy, int heredoc);
void		expand_string(t_shell *shell, char **token_ptr,
				char *copy, int heredoc);
void		expand(t_shell *shell, t_token *list);
//			heredoc
void		destroy_heredocs(t_shell *shell);
void		handle_heredoc(t_shell *shell, t_token **token_list, char *token);
char		*create_heredoc(t_shell *shell, char *delimiter, int expand);
//			parser
int			parse(t_shell *shell, char *input);
//			quote
int			remove_quote_length(char *copy);
void		remove_quotes(t_shell *shell, char **token_ptr, char *copy);
void		unquote(t_shell *shell, t_execution *block);
int			skip_single_quotes(char *input);
int			skip_quotes(char *input);
//			tokenizer
void		print_error(char *error, char *token);
int			check_syntax(t_token *last_token, char *token);
void		add_token(t_shell *shell, t_token **token_list, char *token);
int			tokenizer(t_shell *shell, t_token **token_list, char *input);
//			word split
int			is_splittable(char *str);
void		split_token(t_shell *shell, t_execution *block,
				char *token, int id);
t_token		*ambiguos_redir(t_execution *block, t_token *token_list);
void		word_split(t_shell *shell, t_token *token_list);

/*~~~~~~~~~~~~ UTILS ~~~~~~~~~~~~~~*/

//			char
int			is_alpha(int c);
int			is_alpha_num(int c);
int			is_space(int character);
int			is_separator(char *c);
//			free
void		free_duoble_ptr(t_shell *shell, char **ptr);
void		free_token_list(t_shell *shell, t_token *list);
void		free_execution_list(t_shell *shell, t_execution *list);
void		free_shell(t_shell *shell);
//			itoa
int			ft_size_num(int n);
void		ft_fillnum(char *num, int n, int i, int q);
char		*itoa(t_shell *shell, int n);
//			split
int			pre_num_split(char const *s, char c);
int			len_word_split(char const *s, char c);
void		fill_split(char *src, char *dst, int len);
char		**split(t_shell *shell, char const *s, char c);
//			duplicate string
char		*string_duplicate(t_shell *shell, char *string);
char		*str_n_dup(t_shell *shell, const char *string, int n);
char		*string_join(t_shell *shell, char *str1, char *str2);
//			string
int			str_len(const char *string);
int			string_compare(char *s1, char *s2);
int			string_n_compare(char *s1, char *s2, unsigned int n);
char		*find_char(char *string, char c);
//			other
int			skip_spaces(char *str);

/*~~~~~~~~~~~~ OTHER ~~~~~~~~~~~~~~*/

//			init
void		get_paths(t_shell *shell, char **env);
void		copy_env(t_shell *shell, char **env);
void		init_shell(t_shell *shell, char **env);
//			main
void		reset_shell(t_shell *shell);
//			temp
void		print_exec(t_execution *list);
void		print_list(t_token *list);
//			signals
void		termios_setup(t_shell *shell);
void		handle_sig_int(int signum);
void		setup_signals(t_shell *shell);

#endif