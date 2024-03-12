/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 18:01:08 by apintus           #+#    #+#             */
/*   Updated: 2024/03/12 12:20:29 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	ft_exec(char *cmd, t_pipex *pipex)
{
	char	**split;
	char	*tmp;

	split = ft_split(cmd, ' ');
	if (!split)
	{
		ft_putstr_fd("Error: split failed\n", 2);
		exit(1);
	}
	cmd = ft_get_path(split[0], pipex->env);
	if (cmd)
	{
		tmp = cmd;
		cmd = ft_strdup(tmp);
		free(tmp);
		execute_command(cmd, split, pipex->env);
	}
	else
	{
		free_tab(split);
		ft_putstr_fd("Error: command not found\n", 2);
		exit(127);
	}
}

void	execute_command(char *cmd, char **split, char **env)
{
	execve(cmd, split, env);
	free(cmd);
	free_tab(split);
	handle_exec_error();
}

void	handle_exec_error(void)
{
	if (errno == ENOENT)
	{
		ft_putstr_fd("Error: No such file or directory\n", 2);
		exit(127);
	}
	else if (errno == EACCES)
	{
		ft_putstr_fd("Error: Permission denied\n", 2);
		exit(126);
	}
	else
	{
		ft_putstr_fd("Error: An unknown error occurred\n", 2);
		exit(1);
	}
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

char	*ft_find_path(char **split, char *cmd)
{
	int		i;
	char	*path;
	char	*tmp;

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
