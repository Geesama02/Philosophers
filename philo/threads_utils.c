/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 11:28:33 by oait-laa          #+#    #+#             */
/*   Updated: 2024/02/19 14:39:34 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	join_threads(t_vars *vars)
{
	int	i;

	i = 0;
	while (i < vars->nb_philo)
	{
		if (safe_thread_join(vars->philosophers[i].thread, vars))
			return (1);
		i++;
	}
	if (safe_thread_join(vars->monitor, vars))
		return (1);
	return (0);
}

int	start_threads(t_vars *vars)
{
	int	i;

	i = 0;
	while (i < vars->nb_philo)
	{
		if (pthread_create(&vars->philosophers[i].thread, NULL,
				&routine, &vars->philosophers[i]) != 0)
		{
			handle_create_thread_error(vars, i);
			return (1);
		}
		usleep(300);
		i++;
	}
	if (start_monitor(vars, i))
		return (1);
	return (0);
}

int	start_monitor(t_vars *vars, int i)
{
	if (pthread_create(&vars->monitor, NULL, &monitor, vars) != 0)
	{
		handle_create_thread_error(vars, i);
		return (1);
	}
	return (0);
}

int	safe_thread_join(pthread_t thread, t_vars *vars)
{
	if (pthread_join(thread, NULL) != 0)
	{
		write(2, "pthread_join() error\n", 22);
		safe_mutex_lock(&vars->mutex, vars);
		vars->stop_simulation = 1;
		safe_mutex_unlock(&vars->mutex, vars);
		clean_memory(vars);
		return (1);
	}
	return (0);
}
