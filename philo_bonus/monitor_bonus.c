/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 11:35:31 by oait-laa          #+#    #+#             */
/*   Updated: 2024/02/24 18:11:48 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*monitor(void *vars)
{
	t_philosopher	*tmp;

	tmp = (t_philosopher *)vars;
	while (1)
	{
		// if (stop_similation(tmp))
		// 	return (NULL);
		// if (check_if_done(tmp))
		// {
		// 	sem_wait(tmp->vars->stop);
		// 	tmp->vars->stop_simulation = 1;
		// 	// sem_post(tmp->vars->death);
		// 	sem_post(tmp->vars->stop);
		// 	if (tmp->vars->nb_philo == tmp->id)
		// 		sem_post(tmp->vars->death);
		// 	// printf("===================> philo %d done\n", tmp->id);
		// 	// exit(0);
		// 	// printf("philo %d monitor done\n", tmp->id);
		// 	return (NULL);
		// }
		// if (check_if_dead(tmp))
		// {
		// 	// printf("philo %d monitor done\n", tmp->id);
		// 	return (NULL);
		// }
	}
	return (NULL);
}

// int	stop_similation(t_philosopher *philo)
// {
// 	sem_wait(philo->vars->stop);
// 	if (philo->vars->stop_simulation == 1)
// 	{
// 		sem_post(philo->vars->stop);
// 		sem_post(philo->vars->forks);
// 		sem_post(philo->vars->forks);
// 		return (1);
// 	}
// 	sem_post(philo->vars->stop);
// 	return (0);
// }

int	check_if_dead(t_philosopher *philo)
{
	long	time_now;

	if (check_if_done(philo))
	{
		sem_wait(philo->vars->stop);
		philo->vars->stop_simulation = 1;
		// sem_post(philo->vars->death);
		sem_post(philo->vars->stop);
		if (philo->vars->nb_philo == philo->id)
		{
			sem_post(philo->vars->death);
		}
		return (1);
	}
	sem_wait(philo->vars->stop);
	sem_wait(philo->vars->eat);
	time_now = time_passed(philo->vars);
	if (time_now - philo->last_time_eat > philo->vars->time_to_die && philo->vars->stop_simulation == 0)
	{
		philo->vars->stop_simulation = 1;
		sem_post(philo->vars->stop);
		sem_post(philo->vars->eat);
		print_msg(philo, "died");
		sem_post(philo->vars->forks);
		sem_post(philo->vars->forks);
		sem_post(philo->vars->death);
		return (1);
	}
	sem_post(philo->vars->eat);
	sem_post(philo->vars->stop);
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
