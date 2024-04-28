/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 11:32:17 by oait-laa          #+#    #+#             */
/*   Updated: 2024/04/28 14:31:16 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	lock_forks(t_philosopher *philo)
{
	if (stop_similation(philo))
		return (0);
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->left_fork->mutex);
		if (stop_similation(philo))
			return (0);
		print_msg(philo, "has taken a fork");
		pthread_mutex_lock(&philo->right_fork->mutex);
		if (stop_similation(philo))
			return (0);
		print_msg(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->right_fork->mutex);
		if (stop_similation(philo))
			return (0);
		print_msg(philo, "has taken a fork");
		pthread_mutex_lock(&philo->left_fork->mutex);
		if (stop_similation(philo))
			return (0);
		print_msg(philo, "has taken a fork");
	}
	return (1);
}

int	eat_routine(t_philosopher *philo)
{
	print_msg(philo, "is eating");
	pthread_mutex_lock(&philo->mutex);
	philo->last_time_eat = time_passed(philo->vars);
	pthread_mutex_unlock(&philo->mutex);
	if (stop_similation(philo))
		return (0);
	pthread_mutex_lock(&philo->mutex);
	if (philo->nb_meals != -1)
		philo->nb_meals++;
	pthread_mutex_unlock(&philo->mutex);
	accurate_usleep(philo->vars->time_to_eat, philo->vars);
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(&philo->left_fork->mutex);
		pthread_mutex_unlock(&philo->right_fork->mutex);
	}
	else
	{
		pthread_mutex_unlock(&philo->right_fork->mutex);
		pthread_mutex_unlock(&philo->left_fork->mutex);
	}
	return (1);
}

int	sleep_routine(t_philosopher *philo)
{
	if (stop_similation(philo))
		return (0);
	print_msg(philo, "is sleeping");
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
	pthread_mutex_lock(&tmp->vars->mutex);
	pthread_mutex_unlock(&tmp->vars->mutex);
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
