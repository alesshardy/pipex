/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 14:21:55 by apintus           #+#    #+#             */
/*   Updated: 2024/03/07 18:12:53 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include  "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <sys/fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_pipex
{
	char	**cmd;
	char	**env;
	char	*infile;
	char	*outfile;
	int		fd[2];
	int		prev_fd;
	int		*pid;
	int		cmd_count;
	int		fd_in;
	int		fd_out;
}	t_pipex;

int		ft_pipex(t_pipex *pipex);
void	handle_child_process(t_pipex *pipex, int i);
void	handle_parent_process(t_pipex *pipex, int i);
void	redirection(t_pipex *pipex, int i);
void	open_files(int i, t_pipex *pipex);

void	ft_exec(char *cmd, t_pipex *pipex);
void	execute_command(char *cmd, char **split, char **env);
void	handle_exec_error(void);
char	*ft_get_path(char *cmd, char **env);
char	*ft_find_path(char **split, char *cmd);

void	ft_init_pipex(int argc, char **argv, char **env, t_pipex *pipex);
int		ft_waitpid(t_pipex *pipex);
void	ft_free_split(char **split);
void	free_tab(char **tab);

#endif
