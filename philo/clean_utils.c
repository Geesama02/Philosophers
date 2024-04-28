/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 11:21:18 by oait-laa          #+#    #+#             */
/*   Updated: 2024/04/28 14:29:45 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_philo_mutex(t_vars *vars, int nb_philo)
{
	int	i;

	i = 0;
	while (i < nb_philo)
	{
		if (pthread_mutex_destroy(&vars->philosophers[i].mutex) != 0)
		{
			write(2, "pthread_mutex_destroy() error\n", 30);
			return ;
		}
		i++;
	}
}

void	destroy_forks_mutex(t_vars *vars, int nb_philo)
{
	int	i;

	i = 0;
	while (i < nb_philo)
	{
		if (pthread_mutex_destroy(&vars->forks[i].mutex) != 0)
		{
			write(2, "pthread_mutex_destroy() error\n", 30);
			return ;
		}
		i++;
	}
}

void	clean_memory(t_vars *vars)
{
	int	i;

	i = 0;
	while (i < vars->nb_philo)
	{
		if (pthread_mutex_destroy(&vars->philosophers[i].mutex) != 0)
		{
			write(2, "pthread_mutex_destroy() error\n", 30);
			return ;
		}
		if (pthread_mutex_destroy(&vars->forks[i].mutex) != 0)
		{
			write(2, "pthread_mutex_destroy() error\n", 30);
			return ;
		}
		i++;
	}
	if (pthread_mutex_destroy(&vars->mutex) != 0)
	{
		write(2, "pthread_mutex_destroy() error\n", 30);
		return ;
	}
	free(vars->philosophers);
	free(vars->forks);
}

int	handle_create_thread_error(t_vars *vars, int i)
{
	write(2, "pthread_create() error\n", 24);
	pthread_mutex_lock(&vars->mutex);
	vars->stop_simulation = 1;
	pthread_mutex_unlock(&vars->mutex);
	while (i >= 0)
	{
		if (safe_thread_join(vars->philosophers[i].thread, vars))
			return (1);
		i--;
	}
	clean_memory(vars);
	return (1);
}
