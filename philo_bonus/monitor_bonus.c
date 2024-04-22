/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 11:35:31 by oait-laa          #+#    #+#             */
/*   Updated: 2024/04/22 17:47:24 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*monitor_death(void *vars)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)vars;
	safe_sem_wait(philo->vars->death, philo);
	philo->vars->stop_simulation = 1;
	return (NULL);
}

void	*monitor(void *vars)
{
	t_philosopher	*tmp;

	tmp = (t_philosopher *)vars;
	while (1)
	{
		// if (stop_similation(tmp))
		// 	return (NULL);
		if (check_if_done(tmp))
		{
			if (tmp->vars->nb_philo == tmp->id)
			{
				while (tmp->vars->nb_philo > 0)
				{
					safe_sem_post(tmp->vars->death,
						&tmp->vars->philosophers[tmp->vars->nb_philo--]);
				}
			}
			return (NULL);
		}
		if (check_if_dead(tmp))
		{
			// safe_sem_wait(tmp->vars->stop, tmp);
			safe_sem_post(tmp->vars->stop, tmp);
			// print_msg(tmp, "died");
			
			exit(2);
		}
	}
	return (NULL);
}

int	stop_similation(t_philosopher *philo)
{
	safe_sem_wait(philo->vars->stop, philo);
	if (philo->vars->stop_simulation == 1)
	{
		safe_sem_post(philo->vars->stop, philo);
		return (1);
	}
	safe_sem_post(philo->vars->stop, philo);
	return (0);
}

int	check_if_dead(t_philosopher *philo)
{
	long	time_now;
	int		i;

	i = 0;
	safe_sem_wait(philo->vars->eat_time, philo);
	time_now = time_passed(philo->vars);
	if (time_now - philo->last_time_eat > philo->vars->time_to_die)
	{
		safe_sem_post(philo->vars->eat_time, philo);
		philo->is_dead = 1;
		while (i < philo->vars->nb_philo)
		{
			safe_sem_post(philo->vars->death, &philo->vars->philosophers[i]);
			i++;
		}
		safe_sem_post(philo->vars->forks, philo);
		safe_sem_post(philo->vars->forks, philo);
		return (1);
	}
	safe_sem_post(philo->vars->eat_time, philo);
	return (0);
}

int	check_if_done(t_philosopher *philo)
{
	safe_sem_wait(philo->vars->eat, philo);
	if (philo->nb_meals == -1)
	{
		safe_sem_post(philo->vars->eat, philo);
		return (0);
	}
	if (philo->nb_meals < philo->vars->nb_meals)
	{
		safe_sem_post(philo->vars->eat, philo);
		return (0);
	}
	safe_sem_post(philo->vars->eat, philo);
	return (1);
}
