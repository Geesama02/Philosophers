/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 11:41:07 by oait-laa          #+#    #+#             */
/*   Updated: 2024/04/28 14:31:26 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	time_now(t_vars *vars)
{
	struct timeval	tv;
	long			time;

	if (gettimeofday(&tv, NULL) != 0)
	{
		write(2, "gettimeofday() error\n", 22);
		pthread_mutex_lock(&vars->mutex);
		vars->stop_simulation = 1;
		pthread_mutex_unlock(&vars->mutex);
		return (-1);
	}
	time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (time);
}

void	accurate_usleep(long m_sec, t_vars *vars)
{
	long	start;

	if (m_sec <= 0)
		return ;
	start = time_now(vars);
	while ((time_now(vars) - start) < m_sec)
		usleep(500);
}

long	time_passed(t_vars *vars)
{
	long	time_passed;
	long	time;

	time = time_now(vars);
	time_passed = time - vars->start_time;
	return (time_passed);
}
