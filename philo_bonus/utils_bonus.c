/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 13:09:14 by oait-laa          #+#    #+#             */
/*   Updated: 2024/04/29 10:22:45 by oait-laa         ###   ########.fr       */
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

void	unlink_sem(void)
{
	if (sem_unlink("forks"))
		write(2, "sem_unlink() error\n", 19);
	if (sem_unlink("stop"))
		write(2, "sem_unlink() error\n", 19);
	if (sem_unlink("print"))
		write(2, "sem_unlink() error\n", 19);
	if (sem_unlink("eat"))
		write(2, "sem_unlink() error\n", 19);
	if (sem_unlink("eat_time"))
		write(2, "sem_unlink() error\n", 19);
	if (sem_unlink("death"))
		write(2, "sem_unlink() error\n", 19);
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
