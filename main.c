/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:54:59 by oait-laa          #+#    #+#             */
/*   Updated: 2024/02/16 14:50:24 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *test()
{
	printf("Hello from thread\n");
	return (NULL);
}

int check_argv(int argc, char **argv)
{
	int i;
	int j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

void init_forks(t_vars *vars)
{
	int i = 0;
	vars->forks = malloc(sizeof(t_fork) * vars->nb_philo);
	while(i < vars->nb_philo)
	{
		pthread_mutex_init(&vars->forks[i].mutex, NULL);
		vars->forks[i].id = i;
		vars->forks[i].is_taken = 0;
		i++;
	}
}
long	time_now(void)
{
	struct timeval	tv;
	long			time;
	if (gettimeofday(&tv, NULL) != 0)
		write(2, "gettimeofday() error\n", 22);
	
	time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (time);
}
void accurate_usleep(long m_sec)
{
	long	start;

	start = time_now();
	while ((time_now() - start) < m_sec)
		usleep(500);
	// return (0);
}

int check_if_dead(t_philosopher *philo, long time_now)
{
	int i = 0;
	while (i < philo->vars->nb_philo)
	{
		pthread_mutex_lock(&philo->vars->philosophers[i].mutex);
		if (philo->vars->philosophers[i].is_dead == 1)
		{
			pthread_mutex_unlock(&philo->vars->philosophers[i].mutex);
			pthread_mutex_unlock(&philo->left_fork->mutex);
			pthread_mutex_unlock(&philo->right_fork->mutex);
			return (0);
		}
		pthread_mutex_unlock(&philo->vars->philosophers[i].mutex);
		i++;
	}
	// pthread_mutex_lock(&philo->vars->mutex);
	if(time_now - philo->last_time_eat > philo->vars->time_to_die)
	{
		printf("time_now: %ld | last_time_eat: %ld | time passed: %ld\n", time_now, philo->last_time_eat, time_now - philo->last_time_eat);
		pthread_mutex_lock(&philo->mutex);
		philo->is_dead = 1;
		pthread_mutex_unlock(&philo->mutex);
		pthread_mutex_unlock(&philo->mutex);
		printf("%ld %d is dead\n", time_now, philo->id);
		pthread_mutex_unlock(&philo->left_fork->mutex);
		pthread_mutex_unlock(&philo->right_fork->mutex);
		return (0);
	}
	return (1);
}

long	time_passed(long start_time)
{
	struct timeval tv;
	long time_passed;
	if (gettimeofday(&tv, NULL) != 0)
		write(2, "gettimeofday() error\n", 22);
	time_passed = (tv.tv_sec * 1000 + tv.tv_usec / 1000) - start_time;
	return (time_passed);
}

int	check_if_done(t_vars *vars)
{
	int i = 0;
	if (vars->nb_meals == -1)
		return (0);
	while (i < vars->nb_philo)
	{
		// pthread_mutex_lock(&vars->philosophers[i].mutex);
		if (vars->philosophers[i].nb_meals < vars->nb_meals)
		{
			// printf("philo %d meals: %d\n", vars->philosophers[i].id, vars->philosophers[i].nb_meals);
			// pthread_mutex_unlock(&vars->philosophers[i].mutex);
			return (0);
		}
		// pthread_mutex_unlock(&vars->philosophers[i].mutex);
		i++;
	}
	return (1);
}

void *routine(void *philo)
{
	t_philosopher *tmp = (t_philosopher *)philo;
	// if (tmp->id % 2 == 0)
	// 	accurate_usleep(1);
	// long time_now = time_passed(tmp->vars->start_time);
	while(tmp->vars->stop_simulation != 1)
	{
		// if (tmp->id % 2 != 0)
		// 	usleep(1);
		// usleep(300);
		if (tmp->id % 2 != 0)
		{
			pthread_mutex_lock(&tmp->left_fork->mutex);
			tmp->left_fork->is_taken = tmp->id;
			printf("%ld %d has taken left fork | %d |\n", time_passed(tmp->vars->start_time), tmp->id, tmp->left_fork->id);
			pthread_mutex_lock(&tmp->right_fork->mutex);
			printf("%ld %d has taken right fork | %d |\n", time_passed(tmp->vars->start_time), tmp->id, tmp->right_fork->id);
			tmp->right_fork->is_taken = tmp->id;
		}
		else
		{
			pthread_mutex_lock(&tmp->right_fork->mutex);
			tmp->right_fork->is_taken = tmp->id;
			printf("%ld %d has taken right fork | %d |\n", time_passed(tmp->vars->start_time), tmp->id, tmp->right_fork->id);
			pthread_mutex_lock(&tmp->left_fork->mutex);
			printf("%ld %d has taken left fork | %d |\n", time_passed(tmp->vars->start_time), tmp->id, tmp->left_fork->id);
			tmp->left_fork->is_taken = tmp->id;
		}
		if (tmp->vars->stop_simulation == 1)
		{
			pthread_mutex_unlock(&tmp->left_fork->mutex);
			pthread_mutex_unlock(&tmp->right_fork->mutex);
			return (NULL);
		}
		// printf("%ld %d has taken a fork\n", time_passed(tmp->vars->start_time), tmp->id);
		if (tmp->vars->stop_simulation == 1)
		{
			pthread_mutex_unlock(&tmp->left_fork->mutex);
			pthread_mutex_unlock(&tmp->right_fork->mutex);
			return (NULL);
		}
		printf("%ld %d is eating\n", time_passed(tmp->vars->start_time), tmp->id);
		pthread_mutex_lock(&tmp->mutex);
		tmp->last_time_eat = time_passed(tmp->vars->start_time);
		pthread_mutex_unlock(&tmp->mutex);
		accurate_usleep(tmp->vars->time_to_eat);
		pthread_mutex_lock(&tmp->mutex);
		if (tmp->nb_meals != -1)
			tmp->nb_meals++;
		tmp->has_eaten++;
		pthread_mutex_unlock(&tmp->mutex);
		if(tmp->id % 2 == 0)
		{
			pthread_mutex_unlock(&tmp->left_fork->mutex);
			tmp->left_fork->is_taken = 0;
			pthread_mutex_unlock(&tmp->right_fork->mutex);
			tmp->right_fork->is_taken = 0;
		}
		else
		{
			pthread_mutex_unlock(&tmp->right_fork->mutex);
			tmp->right_fork->is_taken = 0;
			pthread_mutex_unlock(&tmp->left_fork->mutex);
			tmp->left_fork->is_taken = 0;
		}
		if (tmp->vars->stop_simulation == 1)
		{
			pthread_mutex_unlock(&tmp->left_fork->mutex);
			pthread_mutex_unlock(&tmp->right_fork->mutex);
			return (NULL);
		}
		printf("%ld %d is sleeping\n", time_passed(tmp->vars->start_time), tmp->id);
		accurate_usleep(tmp->vars->time_to_sleep);
		if (tmp->vars->stop_simulation == 1)
		{
			pthread_mutex_unlock(&tmp->left_fork->mutex);
			pthread_mutex_unlock(&tmp->right_fork->mutex);
			return (NULL);
		}
		printf("%ld %d is thinking\n", time_passed(tmp->vars->start_time), tmp->id);
	}
	return NULL;
}

void init_philos(t_vars *vars)
{
	int i = 0;
	vars->philosophers = malloc(sizeof(t_philosopher) * vars->nb_philo);
	i = 0;
	while (i < vars->nb_philo)
	{
		vars->philosophers[i].id = i + 1;
		pthread_mutex_init(&vars->philosophers[i].mutex, NULL);
		if(i == vars->nb_philo - 1)
		{
			vars->philosophers[i].left_fork = &vars->forks[i];
			vars->philosophers[i].prev = &vars->philosophers[i - 1];
			vars->philosophers[i].next = &vars->philosophers[0];
			vars->philosophers[i].right_fork = &vars->forks[0];
		}
		else
		{
			vars->philosophers[i].left_fork = &vars->forks[i];
			if(i == 0)
				vars->philosophers[i].prev = &vars->philosophers[vars->nb_philo - 1];
			else
				vars->philosophers[i].prev = &vars->philosophers[i - 1];
			vars->philosophers[i].next = &vars->philosophers[i + 1];
			vars->philosophers[i].right_fork = &vars->forks[i + 1];
		}
		if (vars->nb_meals != -1)
			vars->philosophers[i].nb_meals = 0;
		else
			vars->philosophers[i].nb_meals = -1;
		vars->philosophers[i].vars = vars;
		vars->philosophers[i].must_eat = 0;
		vars->philosophers[i].has_eaten = 0;
		vars->philosophers[i].last_time_eat = 0;
		vars->philosophers[i].is_dead = 0;
		i++;
	}
}

// monitor if a hilosopher is dead

void *monitor(void *vars)
{
	t_vars *tmp = (t_vars *)vars;
	int i = 0;
	long time_now;
	while(1)
	{
		if(check_if_done(tmp))
		{
			pthread_mutex_lock(&tmp->mutex);
			tmp->stop_simulation = 1;
			pthread_mutex_unlock(&tmp->mutex);
			return (NULL);
		}
		while(i < tmp->nb_philo)
		{
			time_now = time_passed(tmp->start_time);
			if(tmp->nb_philo % 2 != 0
				&& time_now - tmp->philosophers[i].last_time_eat < tmp->time_to_die 
				&& time_now - tmp->philosophers[i].last_time_eat > (tmp->time_to_eat * 2 + tmp->time_to_sleep))
			{
				pthread_mutex_unlock(&tmp->philosophers[i].right_fork->mutex);
				pthread_mutex_unlock(&tmp->philosophers[i].left_fork->mutex);
			}
			if(time_now - tmp->philosophers[i].last_time_eat > tmp->time_to_die)
			{
				printf("left_fork: %d | right_fork: %d\n", tmp->philosophers[i].left_fork->is_taken, tmp->philosophers[i].right_fork->is_taken);
				printf("time_now: %ld | last_time_eat: %ld | time passed: %ld\n", time_now, tmp->philosophers[i].last_time_eat, time_now - tmp->philosophers[i].last_time_eat);
				printf("prev philo: %d | next philo: %d\n", tmp->philosophers[i].prev->id, tmp->philosophers[i].next->id);
				printf("has_eaten: %d | next: %d | prev: %d | prev prev: %d\n", tmp->philosophers[i].has_eaten, tmp->philosophers[i].next->has_eaten, tmp->philosophers[i].prev->has_eaten, tmp->philosophers[i].prev->prev->has_eaten);
				printf("%ld %d is dead\n", time_now, tmp->philosophers[i].id);
				pthread_mutex_lock(&tmp->mutex);
				tmp->stop_simulation = 1;
				pthread_mutex_unlock(&tmp->mutex);
				pthread_mutex_unlock(&tmp->philosophers[i].left_fork->mutex);
				pthread_mutex_unlock(&tmp->philosophers[i].right_fork->mutex);
				return (NULL);
			}
			i++;
		}
		i = 0;
	}
	return (NULL);

}

int main(int argc, char **argv)
{
	// pthread_t thread;
	t_vars vars;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	// vars.start_time = tv.tv_sec;
	if(!check_argv(argc, argv) || argc > 6 || argc < 5)
	{
		printf("Invalid Parameters\n");
		return (1);
	}
	vars.nb_philo = ft_atoi(argv[1]);
	vars.time_to_die = ft_atoi(argv[2]);
	vars.time_to_eat = ft_atoi(argv[3]);
	vars.time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		vars.nb_meals = ft_atoi(argv[5]);
	else
		vars.nb_meals = -1;
	vars.start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	vars.stop_simulation = 0;
	init_forks(&vars);
	init_philos(&vars);
	pthread_mutex_init(&vars.mutex, NULL);
	int i = 0;
	while(i < vars.nb_philo)
	{
		// usleep(500);
		pthread_create(&vars.philosophers[i].thread, NULL, routine, &vars.philosophers[i]);
		usleep(300);
		// printf("philo %d left_fork: %d | right_fork: %d\n", vars.philosophers[i].id, vars.philosophers[i].left_fork->id, vars.philosophers[i].right_fork->id);
		i++;
	}
	pthread_create(&vars.monitor, NULL, &monitor, &vars);
	i = 0;
	while(i < vars.nb_philo)
	{
		
		pthread_join(vars.philosophers[i].thread, NULL);
		// printf("philo %d left_fork: %d | right_fork: %d\n", vars.philosophers[i].id, vars.philosophers[i].left_fork->id, vars.philosophers[i].right_fork->id);
		i++;
	}
	pthread_join(vars.monitor, NULL);
	i = 0;
	while(i < vars.nb_philo)
	{
		pthread_mutex_destroy(&vars.forks[i].mutex);
		pthread_mutex_destroy(&vars.philosophers[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&vars.mutex);
	

	// pthread_create(&thread, NULL, &test, NULL);

	// sleep(1);
	
	return (0);
}
