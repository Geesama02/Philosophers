/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 11:35:31 by oait-laa          #+#    #+#             */
/*   Updated: 2024/02/25 11:15:01 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*monitor(void *vars)
{
	t_philosopher	*tmp;

	tmp = (t_philosopher *)vars;
	while (1)
	{
		if (stop_similation(tmp))
			return (NULL);
		if (check_if_done(tmp))
		{
			sem_wait(tmp->vars->stop);
			tmp->vars->stop_simulation = 1;
			sem_post(tmp->vars->stop);
			if (tmp->vars->nb_philo == tmp->id)
				sem_post(tmp->vars->death);
			return (NULL);
		}
		if (check_if_dead(tmp))
			return (NULL);
	}
	return (NULL);
}

int	stop_similation(t_philosopher *philo)
{
	sem_wait(philo->vars->stop);
	if (philo->vars->stop_simulation == 1)
	{
		sem_post(philo->vars->stop);
		return (1);
	}
	sem_post(philo->vars->stop);
	return (0);
}

int	check_if_dead(t_philosopher *philo)
{
	long	time_now;

	sem_wait(philo->vars->stop);
	sem_wait(philo->vars->eat_time);
	time_now = time_passed(philo->vars);
	if (time_now - philo->last_time_eat > philo->vars->time_to_die && philo->vars->stop_simulation == 0)
	{
		philo->vars->stop_simulation = 1;
		sem_post(philo->vars->stop);
		sem_post(philo->vars->eat_time);
		print_msg(philo, "died");
		sem_post(philo->vars->forks);
		sem_post(philo->vars->forks);
		sem_post(philo->vars->death);
		return (1);
	}
	sem_post(philo->vars->eat_time);
	sem_post(philo->vars->stop);
	return (0);
}

int	check_if_done(t_philosopher *philo)
{
	sem_wait(philo->vars->eat);
	if (philo->nb_meals == -1)
	{
		sem_post(philo->vars->eat);
		return (0);
	}
	// printf("philo %d meals %d\n", philo->id, philo->nb_meals);
	if (philo->nb_meals < philo->vars->nb_meals)
	{
		sem_post(philo->vars->eat);
		return (0);
	}
	sem_post(philo->vars->eat);
	return (1);
}
