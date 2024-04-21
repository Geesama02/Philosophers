/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 14:30:42 by oait-laa          #+#    #+#             */
/*   Updated: 2024/04/21 10:56:32 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	safe_mutex_lock(pthread_mutex_t *mutex, t_vars *vars)
{
	if (pthread_mutex_lock(mutex) != 0)
	{
		write(2, "pthread_mutex_lock() error\n", 28);
		vars->stop_simulation = 1;
	}
}

void	safe_mutex_unlock(pthread_mutex_t *mutex, t_vars *vars)
{
	if (pthread_mutex_unlock(mutex) != 0)
	{
		write(2, "pthread_mutex_unlock() error\n", 30);
		vars->stop_simulation = 1;
	}
}

void	print_msg(t_philosopher *philo, char *msg)
{
	safe_mutex_lock(&philo->vars->mutex, philo->vars);
	if (philo->vars->stop_simulation == 0)
		printf("%ld %d %s\n", time_passed(philo->vars), philo->id, msg);
	safe_mutex_unlock(&philo->vars->mutex, philo->vars);
}
