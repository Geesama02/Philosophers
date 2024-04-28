/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 13:09:14 by oait-laa          #+#    #+#             */
/*   Updated: 2024/04/28 14:19:49 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	clean(t_vars *vars)
{
	free(vars->philosophers);
	if (sem_close(vars->stop) != 0)
		write(2, "sem_close() error\n", 18);
	if (sem_close(vars->print) != 0)
		write(2, "sem_close() error\n", 18);
	if (sem_close(vars->eat) != 0)
		write(2, "sem_close() error\n", 18);
	if (sem_close(vars->eat_time) != 0)
		write(2, "sem_close() error\n", 18);
	if (sem_close(vars->death) != 0)
		write(2, "sem_close() error\n", 18);
	if (sem_close(vars->forks) != 0)
		write(2, "sem_close() error\n", 18);
	return (0);
}

void	wait_processs(t_vars *vars, int *i)
{
	int	status;

	while (waitpid(-1, &status, 0) > 0)
	{
		if (WEXITSTATUS(status) == 2 && vars->stop_simulation == 0)
		{
			vars->stop_simulation = 1;
			printf("%ld %d died\n", time_passed(vars),
				vars->philosophers[*i].id);
		}
	}
}
