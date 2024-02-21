/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 11:35:31 by oait-laa          #+#    #+#             */
/*   Updated: 2024/02/21 17:07:46 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*monitor(void *vars)
{
	t_philosopher	*tmp;

	tmp = (t_philosopher *)vars;
	while (1)
	{
		if (check_if_done(tmp))
		{
			sem_wait(tmp->vars->stop);
			tmp->vars->stop_simulation = 1;
			// sem_post(tmp->vars->death);
			sem_post(tmp->vars->stop);
			return (NULL);
		}
		if (check_if_dead(tmp))
			return (NULL);
	}
	return (NULL);
}

int	stop_similation(t_philosopher *philo)
{
	// printf("stop_simulation: %d\n", philo->vars->stop_simulation);
	sem_wait(philo->vars->stop);
	if (philo->vars->stop_simulation == 1)
	{
		sem_post(philo->vars->stop);
		sem_post(philo->vars->forks);
		// sem_post(philo->vars->death);
		sem_post(philo->vars->forks);
		return (1);
	}
	sem_post(philo->vars->stop);
	return (0);
}

int	check_if_dead(t_philosopher *philo)
{
	int		i;
	long	time_now;

	i = 0;
	time_now = time_passed(philo->vars);
	if (time_now - philo->last_time_eat > philo->vars->time_to_die && !philo->vars->stop_simulation)
	{
		printf("%ld %d is dead\n", time_now, philo->id);
		sem_wait(philo->vars->stop);
		philo->vars->stop_simulation = 1;
		sem_post(philo->vars->stop);
		sem_post(philo->vars->death);
		sem_post(philo->vars->forks);
		sem_post(philo->vars->forks);
		return (1);
	}
	return (0);
}

int	check_if_done(t_philosopher *philo)
{
	int	i;

	i = 0;
	sem_wait(philo->vars->eat);
	if (philo->nb_meals == -1)
	{
		sem_post(philo->vars->eat);
		return (0);
	}
	if (philo->nb_meals < philo->vars->nb_meals)
	{
		sem_post(philo->vars->eat);
		return (0);
	}
	sem_post(philo->vars->eat);
	return (1);
}
