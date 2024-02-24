/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:59:49 by oait-laa          #+#    #+#             */
/*   Updated: 2024/02/24 18:12:28 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
		return (0);
	}
	if (!fill_philo(vars))
		return (0);
	return (1);
}



int	init_values(t_vars *vars, int argc, char **argv)
{
	struct timeval	tv;
	
	gettimeofday(&tv, NULL);
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

void *monitor_death(void *vars)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)vars;
	sem_wait(philo->vars->death);
	sem_post(philo->vars->death);
	// printf("philo %d meals %d\n", philo->id, philo->nb_meals);
	sem_wait(philo->vars->stop);
	philo->vars->stop_simulation = 1;
	sem_post(philo->vars->stop);
	return (NULL);
}

void a()
{
	system("leaks philo_bonus");
}

int main(int argc, char **argv)
{
	t_vars	vars;
	int i;

	i = 0;

	if (!check_argv(argc, argv) || argc > 6 || argc < 5)
	{
		printf("Invalid Parameters\n");
		return (1);
	}
	if (init_values(&vars, argc, argv))
		return (0);
	if (!init_philos(&vars))
		return (0);
	sem_unlink("forks");
	sem_unlink("stop");
	sem_unlink("print");
	sem_unlink("eat");
	sem_unlink("death");
	vars.forks = sem_open("forks", O_CREAT, 0644, vars.nb_philo);
	vars.stop = sem_open("stop", O_CREAT, 0644, 1);
	vars.print = sem_open("print", O_CREAT, 0644, 1);
	vars.eat = sem_open("eat", O_CREAT, 0644, 1);
	vars.death = sem_open("death", O_CREAT, 0644, 0);
	int pid = fork();
	while (i < vars.nb_philo - 1)
	{
		if (pid != 0)
			pid = fork();
		else
			break ;
		i++;
	}
	if (pid == 0)
	{
		pthread_create(&vars.death_thread, NULL, &monitor_death, &vars.philosophers[i]);
		// pthread_create(&vars.monitor_thread, NULL, &monitor, &vars.philosophers[i]);
		routine(&vars.philosophers[i]);
		// pthread_detach(vars.monitor_thread);
		pthread_join(vars.death_thread, NULL);
	}
	while (waitpid(-1, NULL, 0) > 0);
	// printf("philo %d done | pid = %d\n", i+ 1, getpid());
	sem_close(vars.forks);
	sem_close(vars.stop);
	sem_close(vars.print);
	sem_close(vars.eat);
	sem_close(vars.death);
	if (pid != 0)
	{
		sem_unlink("forks");
		sem_unlink("stop");
		sem_unlink("print");
		sem_unlink("eat");
		sem_unlink("death");
	}
	free(vars.philosophers);
	return (0);
}
