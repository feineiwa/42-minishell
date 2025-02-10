/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 09:26:30 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/10 08:35:34 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

typedef struct s_sig	t_g_sig;
typedef struct s_shell	t_shell;

typedef enum e_bool
{
	FALSE,
	TRUE
}					t_bool;

typedef enum e_tok_type
{
	ARGS = 0,
	PIPE,
	INFILE,
	OUTFILE,
	APPEND,
	HEREDOC,
	NONE
}					t_tok_type;

typedef struct s_token
{
	char			*value;
	t_tok_type		type;
	struct s_token	*next;
}					t_token;

typedef struct s_hdoc
{
	char			*del;
	t_bool			expanded;
	struct s_hdoc	*next;
}					t_hdoc;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_cmd
{
	char			**argv;
	int				argc;
	char			*input_file;
	char			*output_file;
	char			*error_file;
	int				flag_err;
	int				append;
	t_hdoc			*hdoc;
	struct s_cmd	*next;
}					t_cmd;

struct				s_sig
{
	int				is_runing;
	int				exit_status;
	int				pipfd[2];
	int				*hdoc_fd;
	int				use_pipe;
	int				old_pipefd;
	t_shell			*shell;
};

struct				s_shell
{
	t_env			*envp;
	t_cmd			*cmd;
};

#endif