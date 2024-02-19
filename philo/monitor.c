/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 11:35:31 by oait-laa          #+#    #+#             */
/*   Updated: 2024/02/19 14:11:47 by oait-laa         ###   ########.fr       */
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
			safe_mutex_lock(&tmp->mutex, tmp);
			tmp->stop_simulation = 1;
			safe_mutex_unlock(&tmp->mutex, tmp);
			return (NULL);
		}
		if (check_if_dead(tmp))
			return (NULL);
	}
	return (NULL);
}

int	stop_similation(t_philosopher *philo)
{
	safe_mutex_lock(&philo->vars->mutex, philo->vars);
	if (philo->vars->stop_simulation == 1)
	{
		safe_mutex_unlock(&philo->vars->mutex, philo->vars);
		safe_mutex_unlock(&philo->left_fork->mutex, philo->vars);
		safe_mutex_unlock(&philo->right_fork->mutex, philo->vars);
		return (1);
	}
	safe_mutex_unlock(&philo->vars->mutex, philo->vars);
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
		safe_mutex_lock(&vars->philosophers[i].mutex, vars);
		if (time_now - vars->philosophers[i].last_time_eat > vars->time_to_die)
		{
			printf("%ld %d is dead\n", time_now, vars->philosophers[i].id);
			safe_mutex_lock(&vars->mutex, vars);
			vars->stop_simulation = 1;
			safe_mutex_unlock(&vars->mutex, vars);
			safe_mutex_unlock(&vars->philosophers[i].mutex, vars);
			safe_mutex_unlock(&vars->philosophers[i].left_fork->mutex, vars);
			safe_mutex_unlock(&vars->philosophers[i].right_fork->mutex, vars);
			return (1);
		}
		safe_mutex_unlock(&vars->philosophers[i].mutex, vars);
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
		safe_mutex_lock(&vars->philosophers[i].mutex, vars);
		if (vars->philosophers[i].nb_meals < vars->nb_meals)
		{
			safe_mutex_unlock(&vars->philosophers[i].mutex, vars);
			return (0);
		}
		safe_mutex_unlock(&vars->philosophers[i].mutex, vars);
		i++;
	}
	return (1);
}
