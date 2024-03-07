/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:06:14 by apintus           #+#    #+#             */
/*   Updated: 2024/03/07 18:08:18 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

/*void	ft_exec(char *cmd, t_pipex *pipex)
{
    char	**split;

    split = ft_split(cmd, ' ');
    if (!split)
    {
        ft_putstr_fd("Error: split failed\n", 2);
        exit(1);
    }
    cmd = ft_get_path(split[0], pipex->env);
    if (cmd)
    {
        char *tmp = cmd;
        cmd = ft_strdup(tmp);
        free(tmp);
        execve(cmd, split, pipex->env);
        free(cmd);
        free_tab(split);
        // If execve returns, it means an error occurred
        if (errno == ENOENT) // No such file or directory
        {
            ft_putstr_fd("Error: No such file or directory\n", 2);
            exit(127);
        }
        else if (errno == EACCES) // Permission denied
        {
            ft_putstr_fd("Error: Permission denied\n", 2);
            exit(126);
        }
        else // Other errors
        {
            ft_putstr_fd("Error: An unknown error occurred\n", 2);
            exit(1);
        }
    }
    else
    {
        free_tab(split);
        ft_putstr_fd("Error: command not found\n", 2);
        exit(127); // Exit with an error code
    }
}*/

//OG
/*int	ft_pipex(t_pipex *pipex)
{
    int	i;
    int max_exit_status = 0;

    i = 0;
    while (i < pipex->cmd_count)
    {
        pipe(pipex->fd);
        pipex->pid[i] = fork();
        if (pipex->pid[i] == 0)
        {
            free(pipex->pid);
            redirection(pipex, i);
            if (i == 0 || i == pipex->cmd_count - 1)
                open_files(i, pipex);
            ft_exec(pipex->cmd[i], pipex);
            exit(127);
        }
        else
        {
            if (i)
                close(pipex->prev_fd);
            pipex->prev_fd = pipex->fd[0];
            close(pipex->fd[1]);
        }
        i++;
    }
    close(pipex->prev_fd);
    max_exit_status = ft_waitpid(pipex);
    return max_exit_status;
}*/

int	main(int argc, char **argv, char **env)
{
	static t_pipex	pipex = {0};
	int				ret;

	if (argc != 5)
	{
		ft_putstr_fd("Error: wrong number of arguments\n", 2);
		return (1);
	}
	ft_init_pipex(argc, argv, env, &pipex);
	ret = ft_pipex(&pipex);
	free(pipex.pid);
	close(pipex.fd[0]);
	close(pipex.fd[1]);
	return (ret);
}
