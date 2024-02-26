/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 11:32:17 by oait-laa          #+#    #+#             */
/*   Updated: 2024/02/26 12:17:29 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	lock_forks(t_philosopher *philo)
{
	if (stop_similation(philo))
		return (0);
	safe_sem_wait(philo->vars->forks, philo);
	if (stop_similation(philo))
	{
		safe_sem_post(philo->vars->forks, philo);
		return (0);
	}
	print_msg(philo, "has taken a fork");
	safe_sem_wait(philo->vars->forks, philo);
	if (stop_similation(philo))
	{
		safe_sem_post(philo->vars->forks, philo);
		safe_sem_post(philo->vars->forks, philo);
		return (0);
	}
	print_msg(philo, "has taken a fork");
	return (1);
}

int	eat_routine(t_philosopher *philo)
{
	print_msg(philo, "is eating");
	safe_sem_wait(philo->vars->eat_time, philo);
	philo->last_time_eat = time_passed(philo->vars);
	safe_sem_post(philo->vars->eat_time, philo);
	if (stop_similation(philo))
		return (0);
	accurate_usleep(philo->vars->time_to_eat, philo->vars);
	if (stop_similation(philo))
		return (0);
	safe_sem_wait(philo->vars->eat, philo);
	if (philo->nb_meals != -1)
		philo->nb_meals++;
	safe_sem_post(philo->vars->eat, philo);
	safe_sem_post(philo->vars->forks, philo);
	safe_sem_post(philo->vars->forks, philo);
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
