/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 18:01:16 by apintus           #+#    #+#             */
/*   Updated: 2024/03/07 18:25:55 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	ft_pipex(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_count)
	{
		pipe(pipex->fd);
		pipex->pid[i] = fork();
		if (pipex->pid[i] == 0)
			handle_child_process(pipex, i);
		else
			handle_parent_process(pipex, i);
		i++;
	}
	close(pipex->prev_fd);
	return (ft_waitpid(pipex));
}

void	handle_child_process(t_pipex *pipex, int i)
{
	free(pipex->pid);
	redirection(pipex, i);
	if (i == 0 || i == pipex->cmd_count - 1)
		open_files(i, pipex);
	ft_exec(pipex->cmd[i], pipex);
	exit(127);
}

void	handle_parent_process(t_pipex *pipex, int i)
{
	if (i)
		close(pipex->prev_fd);
	pipex->prev_fd = pipex->fd[0];
	close(pipex->fd[1]);
}

void	redirection(t_pipex *pipex, int i)
{
	if (i != 0)
	{
		dup2(pipex->prev_fd, 0);
		close(pipex->prev_fd);
	}
	if (i != pipex->cmd_count - 1)
		dup2(pipex->fd[1], 1);
	close(pipex->fd[1]);
	close(pipex->fd[0]);
}

void	open_files(int i, t_pipex *pipex)
{
	if (i == 0)
	{
		pipex->fd_in = open(pipex->infile, O_RDONLY);
		if (pipex->fd_in < 0)
		{
			perror(pipex->infile);
			exit(0);
		}
		dup2(pipex->fd_in, STDIN_FILENO);
		close(pipex->fd_in);
	}
	else if (i == pipex->cmd_count - 1)
	{
		pipex->fd_out = open(pipex->outfile,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (pipex->fd_out < 0)
		{
			perror(pipex->outfile);
			exit(1);
		}
		dup2(pipex->fd_out, STDOUT_FILENO);
		close(pipex->fd_out);
	}
}
