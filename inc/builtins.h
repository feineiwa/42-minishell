/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:39:59 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/05 15:08:27 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

int		ft_cd(t_cmd *cmd, t_env *envp);
int		ft_echo(char **args);
int		ft_env(t_shell *shell, t_cmd *cmd);
int		ft_exit(t_shell *shell, char **argv, int sa_stdin, int sa_stdout);
int		ft_export(t_shell *shell, t_cmd *cmd);
int		ft_unset(t_shell *shell, t_cmd *cmd);
int		ft_pwd(void);

// Utils
char	*ft_getcwd(char *s);
int		ft_add_env(t_env **envp, char *arg);

#endif