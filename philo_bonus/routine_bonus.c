/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 11:32:17 by oait-laa          #+#    #+#             */
/*   Updated: 2024/04/27 16:27:51 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	lock_forks(t_philosopher *philo)
{
	if (stop_similation(philo))
		return (0);
	sem_wait(philo->vars->forks);
	if (stop_similation(philo))
	{
		sem_post(philo->vars->forks);
		return (0);
	}
	print_msg(philo, "has taken a fork");
	sem_wait(philo->vars->forks);
	if (stop_similation(philo))
	{
		sem_post(philo->vars->forks);
		sem_post(philo->vars->forks);
		return (0);
	}
	print_msg(philo, "has taken a fork");
	return (1);
}

int	eat_routine(t_philosopher *philo)
{
	if (stop_similation(philo))
	{
		sem_post(philo->vars->forks);
		sem_post(philo->vars->forks);
		return (0);
	}
	print_msg(philo, "is eating");
	sem_wait(philo->vars->eat_time);
	philo->last_time_eat = time_passed(philo->vars);
	sem_post(philo->vars->eat_time);
	sem_wait(philo->vars->eat);
	if (philo->nb_meals != -1)
		philo->nb_meals++;
	sem_post(philo->vars->eat);
	if (stop_similation(philo))
	{
		sem_post(philo->vars->forks);
		sem_post(philo->vars->forks);
		return (0);
	}
	accurate_usleep(philo->vars->time_to_eat, philo->vars);
	sem_post(philo->vars->forks);
	sem_post(philo->vars->forks);
	return (1);
}

int	sleep_routine(t_philosopher *philo)
{
	print_msg(philo, "is sleeping");
	if (stop_similation(philo))
		return (0);
	accurate_usleep(philo->vars->time_to_sleep, philo->vars);
	if (stop_similation(philo))
		return (0);
	return (1);
}

int	think_routine(t_philosopher *philo)
{
	if (stop_similation(philo))
		return (0);
	print_msg(philo, "is thinking");
	if (philo->vars->nb_philo % 2 != 0 && philo->id % 2)
		accurate_usleep((philo->vars->time_to_eat * 2
				- philo->vars->time_to_sleep) / 2, philo->vars);
	return (1);
}

void	*routine(void *philo)
{
	t_philosopher	*tmp;

	tmp = (t_philosopher *)philo;
	while (1)
	{
		if (!lock_forks(tmp))
			return (NULL);
		if (!eat_routine(tmp))
			return (NULL);
		if (!sleep_routine(tmp))
			return (NULL);
		if (!think_routine(tmp))
			return (NULL);
	}
	return (NULL);
}
