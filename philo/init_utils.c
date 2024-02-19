/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 10:59:12 by oait-laa          #+#    #+#             */
/*   Updated: 2024/02/19 14:09:36 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_values(t_vars *vars, int argc, char **argv)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	vars->nb_philo = ft_atoi(argv[1]);
	vars->nb_philo = ft_atoi(argv[1]);
	vars->time_to_die = ft_atoi(argv[2]);
	vars->time_to_eat = ft_atoi(argv[3]);
	vars->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
	{
		if (ft_atoi(argv[5]) == 0)
			return (1);
		vars->nb_meals = ft_atoi(argv[5]);
	}
	else
		vars->nb_meals = -1;
	vars->start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	vars->stop_simulation = 0;
	return (0);
}

int	init_forks(t_vars *vars)
{
	int	i;

	i = 0;
	vars->forks = malloc(sizeof(t_fork) * vars->nb_philo);
	if (vars->forks == NULL)
	{
		write(2, "malloc() error\n", 15);
		return (0);
	}
	while (i < vars->nb_philo)
	{
		if (pthread_mutex_init(&vars->forks[i].mutex, NULL) != 0)
		{
			write(2, "pthread_mutex_init() error\n", 28);
			free(vars->forks);
			destroy_forks_mutex(vars, i);
			vars->stop_simulation = 1;
			return (0);
		}
		vars->forks[i].id = i;
		i++;
	}
	return (1);
}

static int	fill_philo(t_vars *vars)
{
	int	i;

	i = 0;
	while (i < vars->nb_philo)
	{
		vars->philosophers[i].id = i + 1;
		if (init_mutex(vars, &vars->philosophers[i].mutex, i, vars->nb_philo))
			return (0);
		vars->philosophers[i].left_fork = &vars->forks[i];
		if (i == vars->nb_philo - 1)
			vars->philosophers[i].right_fork = &vars->forks[0];
		else
			vars->philosophers[i].right_fork = &vars->forks[i + 1];
		if (vars->nb_meals != -1)
			vars->philosophers[i].nb_meals = 0;
		else
			vars->philosophers[i].nb_meals = -1;
		vars->philosophers[i].vars = vars;
		vars->philosophers[i].last_time_eat = 0;
		i++;
	}
	return (1);
}

int	init_philos(t_vars *vars)
{
	vars->philosophers = malloc(sizeof(t_philosopher) * vars->nb_philo);
	if (vars->philosophers == NULL)
	{
		write(2, "malloc() error\n", 15);
		free(vars->forks);
		destroy_forks_mutex(vars, vars->nb_philo);
		destroy_philo_mutex(vars, vars->nb_philo);
		return (0);
	}
	if (!fill_philo(vars))
		return (0);
	return (1);
}

int	init_mutex(t_vars *vars, pthread_mutex_t *mutex, int philo, int forks)
{
	if (pthread_mutex_init(mutex, NULL) != 0)
	{
		write(2, "pthread_mutex_init() error\n", 28);
		vars->stop_simulation = 1;
		free(vars->forks);
		free(vars->philosophers);
		destroy_philo_mutex(vars, philo);
		destroy_forks_mutex(vars, forks);
		return (1);
	}
	return (0);
}
