/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:06:14 by apintus           #+#    #+#             */
/*   Updated: 2024/03/08 16:50:16 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

int	main(int argc, char **argv, char **env)
{
	static t_pipex	pipex = {0};
	int				ret;

	if (argc < 5 || ((!ft_strncmp(argv[1], "here_doc", 8)) && argc == 5))
	{
		ft_putstr_fd("Error: wrong number of arguments\n", 2);
		return (1);
	}
	if (!ft_strncmp(argv[1], "here_doc", 8))
	{
		pipex.here_doc = 1;
		ft_here_doc(&pipex, argv[2]);
	}
	ft_init_pipex(argc, argv, env, &pipex);
	ret = ft_pipex(&pipex);
	free(pipex.pid);
	close(pipex.fd[0]);
	close(pipex.fd[1]);
	if (pipex.here_doc == 1)
		unlink("here_doc");
	return (ret);
}
