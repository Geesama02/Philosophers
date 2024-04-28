/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 11:35:31 by oait-laa          #+#    #+#             */
/*   Updated: 2024/04/28 14:30:31 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor(void *vars)
{
	t_vars	*tmp;

	tmp = (t_vars *)vars;
	while (1)
	{
		if (check_if_done(tmp))
		{
			pthread_mutex_lock(&tmp->mutex);
			tmp->stop_simulation = 1;
			pthread_mutex_unlock(&tmp->mutex);
			return (NULL);
		}
		if (check_if_dead(tmp))
			return (NULL);
	}
	return (NULL);
}

int	stop_similation(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->vars->mutex);
	if (philo->vars->stop_simulation == 1)
	{
		pthread_mutex_unlock(&philo->vars->mutex);
		pthread_mutex_unlock(&philo->left_fork->mutex);
		pthread_mutex_unlock(&philo->right_fork->mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->vars->mutex);
	return (0);
}

int	check_if_dead(t_vars *vars)
{
	int		i;
	long	time_now;

	i = 0;
	while (i < vars->nb_philo)
	{
		time_now = time_passed(vars);
		pthread_mutex_lock(&vars->philosophers[i].mutex);
		if (time_now - vars->philosophers[i].last_time_eat > vars->time_to_die)
		{
			print_msg(&vars->philosophers[i], "died");
			pthread_mutex_lock(&vars->mutex);
			vars->stop_simulation = 1;
			pthread_mutex_unlock(&vars->mutex);
			pthread_mutex_unlock(&vars->philosophers[i].mutex);
			pthread_mutex_unlock(&vars->philosophers[i].left_fork->mutex);
			pthread_mutex_unlock(&vars->philosophers[i].right_fork->mutex);
			return (1);
		}
		pthread_mutex_unlock(&vars->philosophers[i].mutex);
		i++;
	}
	return (0);
}

int	check_if_done(t_vars *vars)
{
	int	i;

	i = 0;
	if (vars->nb_meals == -1)
		return (0);
	while (i < vars->nb_philo)
	{
		pthread_mutex_lock(&vars->philosophers[i].mutex);
		if (vars->philosophers[i].nb_meals < vars->nb_meals)
		{
			pthread_mutex_unlock(&vars->philosophers[i].mutex);
			return (0);
		}
		pthread_mutex_unlock(&vars->philosophers[i].mutex);
		i++;
	}
	return (1);
}
