/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:27:47 by awaegaer          #+#    #+#             */
/*   Updated: 2025/10/10 16:12:11 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// includes
# include "../libft/inc/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <limits.h>

# define HD_FD 255

//--structs--//
typedef struct s_gc_node
{
	void						*ptr;
	struct s_gc_node			*next;
}								t_gc_node;

typedef struct s_in_out_ctrs
{
	int							in_ctr;
	int							out_ctr;
}								t_in_out_ctrs;

typedef struct s_gc
{
	t_gc_node					*head;
}								t_gc;

typedef struct s_redir
{
	char						*file;
	int							type;
	struct s_redir				*next;
}								t_redir;

typedef enum e_tokentype
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND,
}								t_tokentype;

typedef enum e_quotes_state
{
	NONE,
	SQ,
	DQ,
}								t_quotes_state;

typedef struct s_tokenlist
{
	char						*token;
	char						*qmask;
	int							is_empty_quoted;
	int							is_delim;
	int							token_heredoc_quoted;
	t_tokentype					token_type;
	struct s_tokenlist			*next;
}								t_tokenlist;

typedef struct s_cmd
{
	char						**args;
	char						**infile;
	char						**outfile;
	pid_t						pid;
	int							append;
	int							heredoc;
	int							pipe_in;
	int							pipe_out;
	int							in_is_file;
	int							*in_is_tmp;
	int							is_heredoc_delim_quoted;
	struct s_cmd				*next;
}								t_cmd;

typedef struct s_env
{
	char						*key;
	char						*value;
	struct s_env				*next;
}								t_env;

typedef struct s_shell
{
	t_env						*env;
	int							last_status;
}								t_shell;

typedef struct s_pipe_ctx
{
	char						**envp;
	pid_t						*pid;
	int							*next_in;
}								t_pipe_ctx;

typedef struct s_expand
{
	t_tokenlist					*token;
	char						*new_token;
	char						*new_mask;
	int							i;
	int							start;
}								t_expand;

typedef struct s_minishell
{
	char						*line;
	int							tkl_size;
	t_tokenlist					*tokens;
	t_cmd						*cmds;
	size_t						len;
	t_shell						*shell;
}								t_minishell;

//-----------builtins-----------//

int								builtin_cd(char **args);
int								builtin_echo(char **args);
int								builtin_exit(char **args);
int								builtin_pwd(char **args);
int								builtin_env(char **args);
int								builtin_unset(char **args, t_env **env);
int								builtin_export(char **args, t_env **env);
int								has_cmd(const t_cmd *cmd);

//--export.c--//
int								env_list_size(t_env *env);
t_env							**env_to_sorted_array(t_env *env);
void							sort_env_array(t_env **arr, int size);
void							print_export(t_env *env);
int								is_valid_identifier(char *str);

//-----------executor-----------//

//--builtins.c--//
int								is_builtin(char *cmd);
int								exec_builtin(char **args);

//--executor_heredoc.c--//
char							*hd_read_line(void);
t_tokenlist						*hd_creat_token(char *line);
char							*hd_process_line(char *line,
									int is_delim_quoted);
void							hd_child_process(int fdw, const char *delim,
									int is_delim_quoted);
int								hd_fill_tmp(int fdw, const char *delim,
									int is_delim_quoted);

//--executor_redir.c--//
int								apply_redirection(t_cmd *cmd,
									int with_redirection);
int								prepare_hd_tmp(t_tokenlist *tk);
int								hd_write_line(int fd, const char *s);
int								apply_outfile(t_cmd *cmd, int with_pipe);
//--executor_utils.c--//
int								cmd_has_redir(t_cmd *cmd);
// int								open_only_redir(t_cmd *cmd);
int								wait_all(pid_t *pids, int count);
int								hd_process_input(int fdw, const char *delim,
									int is_delim_quoted);
//--executor_utils.c--//
char							*build_tmp_path(char *base, char *pid_str,
									char *count_str);
char							*generate_tmp_name(void);
int								handle_hd_error(int fdtmp, char *tmp_tpl);
int								open_and_fill_tmp(char *tmp_tpl,
									t_tokenlist *tk);

//--executor_run.c--//
int								exec_builtin_or_fork(t_cmd *cmd, char **envp,
									int has_redir);
void							child_exec_builtin_or_execve(t_cmd *cmd,
									char **envp);
void							handle_cmd_path(t_cmd *cmd, char **envp);

//--executor.c--//
int								wait_status(pid_t pid);
int								run_cmdlist(t_cmd *cmds, char **envp);
int								exec_no_pipeline(t_cmd *cmd, char **envp);

//--pipe_utils.c--//
int								run_pipeline(t_cmd *cmds, char **envp);

//--pipe.c--//
int								pipe_error(const char *msg, int code);
int								spawn_child_and_close(t_cmd *cmd, int in_fd,
									int out_fd, t_pipe_ctx *ctx);

//--signals.c--//
extern volatile sig_atomic_t	g_signal_received;
void							sig_handle(int sig);
void							sig_handle_heredoc(int sig);
void							init_sig(void);
void							init_sig_heredoc(void);
void							sig_handle_quit(int sig);

//--signals2.c--//
void							init_sig_exec(void);

//-----------parser-----------//

// --parser.c--//
t_cmd							*parse_tokenlist(t_tokenlist *tokenlist);
//--parser_token_steps.c--//
int								count_words_segment(t_tokenlist *tmp);
int								consume_redir_in_out(t_cmd *cmdlist,
									t_tokenlist **ptk, int *outfile_counter,
									int *infile_counter);
int								consume_append_or_heredoc(t_cmd *cmdlist,
									t_tokenlist **ptk, int *outfile_counter,
									int *infile_counter);
int								handle_token_step(t_cmd *cmdlist,
									t_tokenlist **ptk, int *i,
									t_in_out_ctrs *in_out_ctrs);
//--parser_pipe.c--//
int								handle_pipe_token(t_cmd *cmdlist,
									t_tokenlist **ptk, int *pipe_before);
int								syntax_error_redir(void);
int								pipe_at_end(int pipe_before);
int								validate_segment_headless(t_cmd *cmdlist,
									int pipe_before);
int								init_args_array(t_cmd *cmdlist,
									t_tokenlist *tk);
//--tokenizer.c--//
t_tokenlist						*init_tokenlist(void);
int								new_token(t_tokenlist **tokenlist,
									int *tkl_size);
int								tokenizer_loop(char *line,
									t_tokenlist *tokenlist, int *tkl_size);
t_tokenlist						*tokenize_line(char *line, int *tkl_size);
//--tokenizer_quote_spc_utils.c--//
int								handle_space(t_tokenlist **tokenlist,
									int *tkl_size, int i);
void							handle_single_quote(t_tokenlist *tkl,
									int *state, int *i, char *line);
void							handle_double_quote(t_tokenlist *tkl,
									int *state, int *i, char *line);
void							append_char_with_mask(t_tokenlist *tokenlist,
									char c, int state);
int								process_quote_char(char *line,
									t_tokenlist *tokenlist, int *i, int *state);
//--tokenizer_loop_utils.c--//
int								handle_error_no_mquote(int state);
int								handle_quotes(char *line,
									t_tokenlist *tokenlist, int *i);
void							consume_heredoc_append(t_tokenlist *tkl,
									char *line, int *i, char sep);
int								handle_pipe_n_redir(char *line,
									t_tokenlist **tokenlist, int *tkl_size,
									int i);
void							handle_heredoc_delim(t_tokenlist **tkl,
									int *tkl_s, char *line, int i);

//--expand.c--//
void							exp_init(t_expand *exp, t_tokenlist *token);
void							expand_one(t_tokenlist *token);
void							expand_tokens(t_tokenlist *tokens);

//--expand_is_n_append.c--//
int								is_sq(char m);
int								is_var_start(char c);
int								is_var_char(char c);
void							append_slice(t_expand *exp);
void							append_mask_repeated(char **dst_m, char mask,
									const char *val);
//--expand_var_utils.c--//
char							*expand_key(const char *key);
int								handle_special_vars(t_expand *exp);
int								consume_var_len(t_expand *exp);
void							expand_regular_var(t_expand *exp, int len);

//-- expand_utils.c--//
int								check_empty_quotes(t_expand *exp);
int								try_expand_special_vars(t_expand *exp);

//-----------utils-----------//

//--env_utils.c--//
void							set_env_value(t_env **env, const char *key,
									const char *value);
void							unset_env_var(t_env **env, const char *key);

//--env.c--//
void							free_env(t_env **env);
t_env							*init_env(char **envp);
char							*get_env_value(t_env *env, const char *key);
//--env2.c--//
char							**env_to_array(t_env *env);
//--utils.c--//
char							*ft_strjoin_three(const char *s1,
									const char *s2, const char *s3);
t_shell							*get_shell(void);
void							cleanup_and_exit(int code, int do_exit);
//--garbage_collector.c--//
t_gc							*get_gc(void);
void							*gc_malloc(size_t size);
char							*gc_strdup(const char *s);
void							gc_free_all(void);
char							*gc_substr(char const *s, unsigned int start,
									size_t len);
//--garbage_collector_utils.c--//
char							*gc_strjoin_char(char const *s1, char const c);
char							*gc_strdup_until_charset(const char *src,
									const char *charset);
char							*gc_strjoin_len(char *s1, const char *s2,
									size_t len);
char							*gc_itoa(int n);

//--garbage_collector_utils2.c--//
char							*gc_strjoin(char const *s1, char const *s2);

//--strutils.c--//
int								ft_strcmp(const char *s1, const char *s2);

//-----------src-----------//

// void							print_banner(void);
// int								main_loop_test(char *argv, char **envp);
int								handle_tokens(char *line, t_tokenlist **tokens,
									int *tkl_size);
int								handle_cmds(t_minishell *ml);
int								handle_heredocs(t_tokenlist **ptk);
int								handle_parsing(char *line, t_tokenlist **tokens,
									int *tkl_size);
int								handle_execution(t_minishell *ml, char **envp);
int								main_loop(char **envp);
int								main(int argc, char **argv, char **envp);

// void		test_env_functions(char **envp);
// void 		rint_env(void);
void							print_cmdlist(t_cmd *cmdlist);

#endif
