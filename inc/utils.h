/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 09:46:39 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 16:45:49 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

// ENV_SHELL
t_shell	init_shell(char **envp);
void	ft_swap_env(t_env *i, t_env *j, t_env *tmp);
char	**convert_env_to_array(t_env *envp);

// ERROR_UTILS
t_bool	synthax_error(t_token *tok);
void	print_err(char *s1, char *s2, char *s3, int fd);
void	print_synthax_error(t_cmd **cmd_list);
int		update_pwd(t_env *envp, char *path);
int		update_oldpwd(t_env *envp, char *old_pwd);

// EXTRA_LIBFT
int		ft_skip_space(char *str);
int		ft_search_equ(char *s);
int		ft_is_quote(char c);
int		ft_isspace(char c);
int		ft_strcmp(char *s1, char *s2);

char	*ft_strjoin_free(char *s1, char *s2);
char	*ft_strjoin_s1(char *s1, char *s2);
char	*ft_strjoin3(char *s1, char *s2, char *s3);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
char	*ft_strndup(char *str, size_t n);
int		ft_cmdsize(t_cmd *cmd);
int		ft_is_belong(char c);
t_bool	ft_is_special(char c);
char	*add_double_quotes(char *s);
int		is_only_dot_or_slash(char *s);

// ENV_UTILS
t_env	*add_new_env(char *key, char *value);
char	*ft_get_env_value(t_env *envp, char *key);
void	ft_unset_env(t_env **envp, char *key);
t_env	*ft_get_last_env(t_env *envp);

// FT_FREE
void	ft_free_all(t_shell *shell);
void	ft_free_arr(char **arr);
void	ft_free_one_env(t_env *tmp);
void	ft_free(void *ptr);

void	ft_free_env(t_env **envp);
void	ft_free_cmd(t_cmd **cmd);
void	ft_free_hdoc(t_hdoc **hdoc);
void	ft_free_arg(char **argv);
void	ft_free_token(t_token *tok);
#endif