/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 17:40:36 by apintus           #+#    #+#             */
/*   Updated: 2024/03/07 18:19:58 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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

// Extract the exit status
// Return the exit status of the last command
int	ft_waitpid(t_pipex *pipex)
{
	int	i;
	int	status;
	int	exit_statuses[pipex->cmd_count];

	i = 0;
	while (i < pipex->cmd_count)
	{
		waitpid(pipex->pid[i], &status, 0);
		exit_statuses[i] = status >> 8;
		i++;
	}
	return (exit_statuses[pipex->cmd_count - 1]);
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
