/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 11:35:31 by oait-laa          #+#    #+#             */
/*   Updated: 2024/04/28 12:01:55 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*monitor_death(void *vars)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)vars;
	sem_wait(philo->vars->death);
	sem_post(philo->vars->death);
	if (philo->is_dead == 1)
		exit(2);
	exit(0);
	return (NULL);
}

void	*monitor(void *vars)
{
	t_philosopher	*tmp;

	tmp = (t_philosopher *)vars;
	while (1)
	{
		if (stop_similation(tmp))
			return (NULL);
		if (check_if_done(tmp))
		{
			if (tmp->vars->nb_philo == tmp->id)
			{
				while (tmp->vars->nb_philo > 0)
				{
					sem_post(tmp->vars->death);
					tmp->vars->nb_philo--;
				}
			}
			return (NULL);
		}
		if (check_if_dead(tmp))
			return (NULL);
	}
	return (NULL);
}

int	stop_similation(t_philosopher *philo)
{
	sem_wait(philo->vars->stop);
	if (philo->vars->stop_simulation == 1)
	{
		sem_post(philo->vars->stop);
		return (1);
	}
	sem_post(philo->vars->stop);
	return (0);
}

int	check_if_dead(t_philosopher *philo)
{
	long	time_now;
	int		i;

	i = 0;
	sem_wait(philo->vars->eat_time);
	time_now = time_passed(philo->vars);
	if (time_now - philo->last_time_eat > philo->vars->time_to_die)
	{
		sem_post(philo->vars->eat_time);
		philo->is_dead = 1;
		while (i < philo->vars->nb_philo)
		{
			sem_post(philo->vars->death);
			i++;
		}
		return (1);
	}
	sem_post(philo->vars->eat_time);
	return (0);
}

int	check_if_done(t_philosopher *philo)
{
	sem_wait(philo->vars->eat);
	if (philo->nb_meals == -1)
	{
		sem_post(philo->vars->eat);
		return (0);
	}
	if (philo->nb_meals < philo->vars->nb_meals)
	{
		sem_post(philo->vars->eat);
		return (0);
	}
	sem_post(philo->vars->eat);
	return (1);
}
