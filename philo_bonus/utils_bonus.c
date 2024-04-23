/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 13:09:14 by oait-laa          #+#    #+#             */
/*   Updated: 2024/04/23 11:39:43 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	clean(t_vars *vars)
{
	if (sem_close(vars->forks) != 0)
		return (1);
	if (sem_close(vars->stop) != 0)
		return (1);
	if (sem_close(vars->print) != 0)
		return (1);
	if (sem_close(vars->eat) != 0)
		return (1);
	if (sem_close(vars->eat_time) != 0)
		return (1);
	if (sem_close(vars->death) != 0)
		return (1);
	free(vars->philosophers);
	return (0);
}

void	safe_sem_wait(sem_t *sem, t_philosopher *philo)
{
	if (sem_wait(sem) != 0 && philo->vars->stop_simulation == 0)
	{
		write(2, "sem_wait() error\n", 17);
		if (clean(philo->vars))
			write(2, "sem_close() error\n", 18);
		sem_post(philo->vars->death);
		exit(1);
	}
}

void	safe_sem_post(sem_t *sem, t_philosopher *philo)
{
	if (sem_post(sem) != 0 && philo->vars->stop_simulation == 0)
	{
		write(2, "sem_post() error\n", 17);
		if (clean(philo->vars))
			write(2, "sem_close() error\n", 18);
		sem_post(philo->vars->death);
		exit(1);
	}
}
