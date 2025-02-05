/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 09:28:50 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 15:08:39 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

// PARSING
t_cmd		*parsing(t_shell *shell, char *input);

int			handle_argument(t_cmd **tmp, t_shell *shell, t_token **tok);
int			handle_input_file(t_cmd **tmp, t_token **tok, int *error_flag,
				t_cmd *cmd_list);
int			handle_output_file(t_cmd **tmp, t_token **tok, int *error_flag,
				t_cmd *cmd_list);
int			handle_append_file(t_cmd **tmp, t_token **tok, int *error_flag,
				t_cmd *cmd_list);
int			handle_heredoc_parsing(t_cmd **tmp, t_token **tok, t_cmd *cmd_list);

int			handle_pipeline(t_cmd **tmp, t_token **tok, int *error_flag,
				t_cmd *cmd_list);
void		set_error_file(t_cmd **tmp, char *file, int flag, int *error_flag);
char		*ft_get_arg(t_shell *shell, char *tok);

// LEXING

t_token		*lexer_input(char *input);

// EXPAND
char		*ft_expand(t_shell *shell, char *s);
char		*ft_expand_for_hdoc(t_shell *shell, char *s);

char		*extract_var(char *s, t_shell *shell);
char		*format_value(char *s);
int			ft_strlen_expand(char *s);

int			ft_is_after_equal(char *s, int i);
t_bool		ft_is_expanded(char *s, int i);
int			ft_is_after_here(char *s, int i);

// PARSE_UTILES
int			count_quotes(char *input);
t_cmd		*init_cmd(t_cmd *cmd);
t_hdoc		*init_hdoc(t_hdoc *hdoc);
t_token		*add_new_token(char *s);
t_tok_type	assign_type(char *s);
t_hdoc		*get_last_hdoc(t_hdoc *first);

int			count_quotes(char *input);
int			ft_is_between(char *str, int index);
int			skip_quotes(char *tok, char *arg, int *i, int *j);
int			ft_strlen_skip_quote(char *s);

#endif