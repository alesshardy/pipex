/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 17:40:36 by apintus           #+#    #+#             */
/*   Updated: 2024/03/08 16:45:56 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

// Initialize the pipex structure
void	ft_init_pipex(int argc, char **argv, char **env, t_pipex *pipex)
{
	if (pipex->here_doc)
	{
		pipex->cmd_count = argc - 4;
		pipex->cmd = &argv[3];
	}
	else
	{
		pipex->cmd_count = argc - 3;
		pipex->cmd = &argv[2];
	}
	pipex->env = env;
	pipex->infile = argv[1];
	pipex->outfile = argv[argc - 1];
	pipex->pid = malloc(sizeof(int) * (pipex->cmd_count));
	if (!pipex->pid)
	{
		ft_putstr_fd("Error: malloc failed\n", 2);
		exit(1);
	}
}

// Extract the exit status
// Return the exit status of the last command
int	ft_waitpid(t_pipex *pipex)
{
	int	i;
	int	status;
	int	*exit_statuses;

	exit_statuses = malloc(pipex->cmd_count * sizeof(int));
	if (!exit_statuses)
	{
		ft_putstr_fd("Error: malloc failed\n", 2);
		return (-1);
	}
	i = 0;
	while (i < pipex->cmd_count)
	{
		waitpid(pipex->pid[i], &status, 0);
		exit_statuses[i] = status >> 8;
		i++;
	}
	status = exit_statuses[pipex->cmd_count - 1];
	free(exit_statuses);
	return (status);
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
