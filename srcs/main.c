/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:06:14 by apintus           #+#    #+#             */
/*   Updated: 2024/03/06 18:03:30 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	ft_waitpid(t_pipex *pipex)
{
    int	i;
    int	status;
    int max_exit_status = 0;

    i = 0;
    while (i < pipex->cmd_count)
    {
        waitpid(pipex->pid[i], &status, 0);
        int exit_status = (status >> 8) & 0xFF; // Extract the exit status
        if (exit_status > max_exit_status)
        {
            max_exit_status = exit_status;
        }
        // ... (rest of your code)
        i++;
    }
    return max_exit_status;
}

void	ft_free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	*ft_find_path(char **split, char *cmd)
{
	int i;
	char *path;
	char *tmp;

	i = 0;
	while (split[i])
	{
		path = ft_strjoin(split[i], "/");
		tmp = path;
		path = ft_strjoin(path, cmd);
		free(tmp);
		if (access(path, F_OK | X_OK) == 0)
		{
			free_tab(split);
			return (path);
		}
		free(path);
		i++;
	}
	free_tab(split);
	return (NULL);
}

char	*ft_get_path(char *cmd, char **env)
{
	int		i;
	char	**split;
	char	*path;

	i = 0;
	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			path = ft_strdup(env[i] + 5);
			split = ft_split(path, ':');
			free(path);
			path = ft_find_path(split, cmd);
			if (path)
				return (path);
		}
		i++;
	}
	return (NULL);
}

void	ft_exec(char *cmd, t_pipex *pipex)
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
        pipex->fd_out = open(pipex->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (pipex->fd_out < 0)
        {
            perror(pipex->outfile);
            exit(1);
        }
        dup2(pipex->fd_out, STDOUT_FILENO);
        close(pipex->fd_out);
    }
}

void	redirection(t_pipex *pipex, int i)
{
	if (i != 0)
	{
		dup2(pipex->prev_fd, 0); //STDIN_FILENO
		close(pipex->prev_fd);
	}
	if (i != pipex->cmd_count - 1)
		dup2(pipex->fd[1], 1);//STDOUT_FILENO
	close(pipex->fd[1]);
	close(pipex->fd[0]);
}

/*void check_file_permissions(t_pipex *pipex)
{
    if (access(pipex->infile, R_OK) != 0)
    {
        ft_putstr_fd("Error: Permission denied\n", 2);
        exit(0);
    }
    if (access(pipex->outfile, W_OK) != 0)
    {
        ft_putstr_fd("Error: Permission denied\n", 2);
        exit(1);
    }
}*/

int	ft_pipex(t_pipex *pipex)
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
}

void	ft_init_pipex(int argc, char **argv, char **env, t_pipex *pipex)
{
	pipex->cmd = &argv[2];
	pipex->env = env;
	pipex->infile = argv[1];
	pipex->outfile = argv[argc - 1];
	pipex->cmd_count = argc - 3;
	pipex->pid = malloc(sizeof(int) * (pipex->cmd_count));
	if (!pipex->pid)
	{
		fprintf(stderr, "Error: malloc failed\n");
		exit(1);
	}
}

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
	//check_file_permissions(&pipex);
	ret = ft_pipex(&pipex);
	free(pipex.pid);
	close(pipex.fd[0]);
	close(pipex.fd[1]);
	return (ret);
}

/*void	ft_waitpid(t_pipex *pipex)
{
	int	i;
	int	status;

	i = 0;
	while (i < pipex->cmd_count)
	{
		waitpid(pipex->pid[i], &status, 0);
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 127)
				exit(127);
		}
		i++;
	}
}

void	ft_free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

char	*ft_get_path(char *cmd, char **env)
{
	int		i;
	char	**split;
	char	*path;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			split = ft_split(env[i] + 5, ':');
			i = 0;
			while (split[i])
			{
				path = ft_strjoin(split[i], "/");
				path = ft_strjoin(path, cmd);
				if (access(path, F_OK) == 0)
				{
					ft_free_split(split);
					return (path);
				}
				free(path);
				i++;
			}
			ft_free_split(split);
		}
		i++;
	}
	return (NULL);
}

void	ft_exec(char *cmd, t_pipex *pipex)
{
	char	*path;
	char	**split;

	split = ft_split(cmd, ' ');
	path = ft_get_path(split[0], pipex->env);
	if (!path)
	{
		ft_putstr_fd("Error: command not found\n", 2);
		exit(127);
	}
	execve(path, split, pipex->env);
	ft_putstr_fd("Error: execve failed\n", 2);
	exit(127);
}

void	open_files(int i, t_pipex *pipex)
{
	if (i == 0)
		pipex->fd[0] = open(pipex->infile, O_RDONLY);
	else
		pipex->fd[0] = open(pipex->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->fd[0] < 0)
	{
		ft_putstr_fd("Error: open failed\n", 2);
		exit(1);
	}
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
	else
	{
		dup2(pipex->fd[1], 1);
		close(pipex->fd[1]);
	}
}

void	ft_pipex(t_pipex *pipex)
{
	int	i;

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
	}
	ft_waitpid(pipex);
}

void	ft_init_pipex(int argc, char **argv, char **env, t_pipex *pipex)
{
	pipex->cmd = argv;
	pipex->env = env;
	pipex->infile = argv[1];
	pipex->outfile = argv[argc - 3];
	pipex->prev_fd = 0;
	pipex->cmd_count = argc - 3;
	pipex->pid = malloc(sizeof(int) * (pipex->cmd_count));
	if (!pipex->pid)
	{
		ft_putstr_fd("Error: malloc failed\n", 2);
		exit(1);
	}
}

int	main(int argc,char **argv, char **env)
{
	t_pipex	pipex;

	if (argc != 5)
	{
		ft_putstr_fd("Error: wrong number of arguments\n", 2);
		return (1);
	}
	ft_init_pipex(argc, argv, env, &pipex);
	ft_pipex(&pipex);
	free(pipex.pid);
	close(pipex.fd[0]);
	close(pipex.fd[1]);
	return (0);
}*/
