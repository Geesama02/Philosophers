/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 12:00:29 by oait-laa          #+#    #+#             */
/*   Updated: 2024/04/27 12:02:44 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	init_values(t_vars *vars, int argc, char **argv)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
	{
		write(2, "gettimeofday() error\n", 21);
		exit(1);
	}
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

int	init_philos(t_vars *vars)
{
	vars->philosophers = malloc(sizeof(t_philosopher) * vars->nb_philo);
	if (vars->philosophers == NULL)
	{
		write(2, "malloc() error\n", 15);
		return (0);
	}
	if (!fill_philo(vars))
		return (0);
	return (1);
}

int	init_semaphores(t_vars *vars)
{
	sem_unlink("forks");
	sem_unlink("stop");
	sem_unlink("print");
	sem_unlink("eat");
	sem_unlink("eat_time");
	sem_unlink("death");
	vars->forks = sem_open("forks", O_CREAT, 0644, vars->nb_philo);
	if (vars->forks == SEM_FAILED)
		return (1);
	vars->stop = sem_open("stop", O_CREAT, 0644, 1);
	if (vars->stop == SEM_FAILED)
		return (1);
	vars->print = sem_open("print", O_CREAT, 0644, 1);
	if (vars->print == SEM_FAILED)
		return (1);
	vars->eat = sem_open("eat", O_CREAT, 0644, 1);
	if (vars->eat == SEM_FAILED)
		return (1);
	vars->eat_time = sem_open("eat_time", O_CREAT, 0644, 1);
	if (vars->eat_time == SEM_FAILED)
		return (1);
	vars->death = sem_open("death", O_CREAT, 0644, 0);
	if (vars->death == SEM_FAILED)
		return (1);
	return (0);
}

int	fill_philo(t_vars *vars)
{
	int	i;

	i = 0;
	while (i < vars->nb_philo)
	{
		vars->philosophers[i].id = i + 1;
		if (vars->nb_meals != -1)
			vars->philosophers[i].nb_meals = 0;
		else
			vars->philosophers[i].nb_meals = -1;
		vars->philosophers[i].vars = vars;
		vars->philosophers[i].is_dead = 0;
		vars->philosophers[i].last_time_eat = 0;
		i++;
	}
	return (1);
}

int	init_threads(t_vars *vars, int i, int pid)
{
	if (pthread_create(&vars->death_thread, NULL,
			&monitor_death, &vars->philosophers[i]) != 0)
	{
		write(2, "pthread_create() error\n", 23);
		safe_sem_post(vars->death, &vars->philosophers[i]);
		clean_mem(vars, pid);
		return (1);
	}
	if (pthread_create(&vars->monitor_thread, NULL,
			&monitor, &vars->philosophers[i]) != 0)
	{
		write(2, "pthread_create() error\n", 23);
		safe_sem_post(vars->death, &vars->philosophers[i]);
		if (pthread_detach(vars->death_thread) != 0)
		{
			write(2, "pthread_detach() error\n", 21);
			safe_sem_post(vars->death, &vars->philosophers[i]);
			clean_mem(vars, pid);
			return (1);
		}
		clean_mem(vars, pid);
		return (1);
	}
	return (0);
}
