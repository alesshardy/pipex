/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 13:07:44 by apintus           #+#    #+#             */
/*   Updated: 2024/03/11 17:13:20 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	ft_here_doc(t_pipex *pipex, char *limiter)
{
	int		fd;
	char	*line;

	fd = open("here_doc", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		ft_putstr_fd("Error: here_doc\n", 2);
	while (1)
	{
		line = get_next_line(0, 0);
		if (!line)
			break ;
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& (ft_strlen(line) - 1) == ft_strlen(limiter))
		{
			free(line);
			get_next_line(0, 1);
			break ;
		}
		ft_putstr_fd(line, fd);
		free(line);
	}
	close(fd);
	pipex->infile = "here_doc";
}
