/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 11:41:07 by oait-laa          #+#    #+#             */
/*   Updated: 2024/04/27 16:30:14 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long	time_now(t_vars *vars)
{
	struct timeval	tv;
	long			time;

	if (gettimeofday(&tv, NULL) != 0)
	{
		write(2, "gettimeofday() error\n", 22);
		sem_post(vars->death);
		exit(1);
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
	{
		if (stop_similation(vars->philosophers))
			return ;
		usleep(500);
	}
}

long	time_passed(t_vars *vars)
{
	long	time_passed;
	long	time;

	time = time_now(vars);
	time_passed = time - vars->start_time;
	return (time_passed);
}

void	print_msg(t_philosopher *philo, char *msg)
{
	sem_wait(philo->vars->print);
	sem_wait(philo->vars->stop);
	if (philo->vars->stop_simulation == 0)
		printf("%ld %d %s\n", time_passed(philo->vars), philo->id, msg);
	sem_post(philo->vars->stop);
	sem_post(philo->vars->print);
}
