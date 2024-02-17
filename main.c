/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:54:59 by oait-laa          #+#    #+#             */
/*   Updated: 2024/02/17 18:09:07 by oait-laa         ###   ########.fr       */
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

int init_forks(t_vars *vars)
{
	int i = 0;
	vars->forks = malloc(sizeof(t_fork) * vars->nb_philo);
	if (vars->forks == NULL)
	{
		write(2, "malloc() error\n", 15);
		return (0);
	}
	while(i < vars->nb_philo)
	{
		if (pthread_mutex_init(&vars->forks[i].mutex, NULL) != 0)
		{
			write(2, "pthread_mutex_init() error\n", 28);
			vars->stop_simulation = 1;
			return (0);
		}
		vars->forks[i].id = i;
		i++;
	}
	return (1);
}
long	time_now(void)
{
	struct timeval	tv;
	long			time;
	if (gettimeofday(&tv, NULL) != 0)
	{
		write(2, "gettimeofday() error\n", 22);
		return (-1);
	}
	
	time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (time);
}
void safe_mutex_lock(pthread_mutex_t *mutex, t_vars *vars)
{
	if (pthread_mutex_lock(mutex) != 0)
	{
		write(2, "pthread_mutex_lock() error\n", 28);
		vars->stop_simulation = 1;
	}
}

void safe_mutex_unlock(pthread_mutex_t *mutex, t_vars *vars)
{
	if (pthread_mutex_unlock(mutex) != 0)
	{
		write(2, "pthread_mutex_unlock() error\n", 30);
		vars->stop_simulation = 1;
	}
}

void accurate_usleep(long m_sec, t_vars *vars)
{
	long	start;

	if(m_sec <= 0)
		return ;
	start = time_now();
	if (start == -1)
	{
		safe_mutex_lock(&vars->mutex, vars);
		vars->stop_simulation = 1;
		safe_mutex_unlock(&vars->mutex, vars);
		return ;
	}
	while ((time_now() - start) < m_sec)
		usleep(500);
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
		safe_mutex_lock(&vars->philosophers[i].mutex, vars);
		if (vars->philosophers[i].nb_meals < vars->nb_meals)
		{
			safe_mutex_unlock(&vars->philosophers[i].mutex, vars);
			return (0);
		}
		safe_mutex_unlock(&vars->philosophers[i].mutex, vars);
		i++;
	}
	return (1);
}

int	stop_similation(t_philosopher *philo)
{
	safe_mutex_lock(&philo->vars->mutex, philo->vars);
	if (philo->vars->stop_simulation == 1)
	{
		safe_mutex_unlock(&philo->vars->mutex, philo->vars);
		safe_mutex_unlock(&philo->left_fork->mutex, philo->vars);
		safe_mutex_unlock(&philo->right_fork->mutex, philo->vars);
		return (1);
	}
	safe_mutex_unlock(&philo->vars->mutex, philo->vars);
	return (0);

}

int lock_forks(t_philosopher *philo)
{
	if (philo->id % 2 == 0)
	{
		safe_mutex_lock(&philo->left_fork->mutex, philo->vars);
		if(stop_similation(philo))
			return (0);
		printf("%ld %d has taken a fork\n", time_passed(philo->vars->start_time), philo->id);
		safe_mutex_lock(&philo->right_fork->mutex, philo->vars);
		if(stop_similation(philo))
			return (0);
		printf("%ld %d has taken a fork\n", time_passed(philo->vars->start_time), philo->id);
	}
	else
	{
		safe_mutex_lock(&philo->right_fork->mutex, philo->vars);
		if(stop_similation(philo))
			return (0);
		printf("%ld %d has taken a fork\n", time_passed(philo->vars->start_time), philo->id);
		safe_mutex_lock(&philo->left_fork->mutex, philo->vars);
		if(stop_similation(philo))
			return (0);
		printf("%ld %d has taken a fork\n", time_passed(philo->vars->start_time), philo->id);
	}
	return (1);
}

int	eat_routine(t_philosopher *philo)
{
	printf("%ld %d is eating\n", time_passed(philo->vars->start_time), philo->id);
	safe_mutex_lock(&philo->mutex, philo->vars);
	philo->last_time_eat = time_passed(philo->vars->start_time);
	safe_mutex_unlock(&philo->mutex, philo->vars);
	if (stop_similation(philo))
		return (0);
	accurate_usleep(philo->vars->time_to_eat, philo->vars);
	safe_mutex_lock(&philo->mutex, philo->vars);
	if (philo->nb_meals != -1)
		philo->nb_meals++;
	safe_mutex_unlock(&philo->mutex, philo->vars);
	if(philo->id % 2 == 0)
	{
		safe_mutex_unlock(&philo->left_fork->mutex, philo->vars);
		safe_mutex_unlock(&philo->right_fork->mutex, philo->vars);
	}
	else
	{
		safe_mutex_unlock(&philo->right_fork->mutex, philo->vars);
		safe_mutex_unlock(&philo->left_fork->mutex, philo->vars);
	}
	return (1);
}

int	sleep_routine(t_philosopher *philo)
{
	if (stop_similation(philo))
		return (0);
	printf("%ld %d is sleeping\n", time_passed(philo->vars->start_time), philo->id);
	accurate_usleep(philo->vars->time_to_sleep, philo->vars);
	if (stop_similation(philo))
		return (0);
	return (1);
}

int think_routine(t_philosopher *philo)
{
	printf("%ld %d is thinking\n", time_passed(philo->vars->start_time), philo->id);
	if (philo->vars->nb_philo % 2 != 0 && philo->id % 2)
		accurate_usleep((philo->vars->time_to_eat * 2 - philo->vars->time_to_sleep) / 2, philo->vars);
	return (1);
}

void *routine(void *philo)
{
	t_philosopher *tmp = (t_philosopher *)philo;

	while(1)
	{
		if (!lock_forks(tmp))
			return (NULL);
		if (!eat_routine(tmp))
			return (NULL);
		if (!sleep_routine(tmp))
			return (NULL);
		if (!think_routine(tmp))
			return (NULL);
	}
	return NULL;
}

int init_philos(t_vars *vars)
{
	int i = 0;
	vars->philosophers = malloc(sizeof(t_philosopher) * vars->nb_philo);
	if (vars->philosophers == NULL)
	{
		write(2, "malloc() error\n", 15);
		return (0);
	}
	i = 0;
	while (i < vars->nb_philo)
	{
		vars->philosophers[i].id = i + 1;
		if (pthread_mutex_init(&vars->philosophers[i].mutex, NULL) != 0)
		{
			write(2, "pthread_mutex_init() error\n", 28);
			return (0);
		}
		if(i == vars->nb_philo - 1)
		{
			vars->philosophers[i].left_fork = &vars->forks[i];
			vars->philosophers[i].right_fork = &vars->forks[0];
		}
		else
		{
			vars->philosophers[i].left_fork = &vars->forks[i];
			vars->philosophers[i].right_fork = &vars->forks[i + 1];
		}
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

// monitor if a hilosopher is dead

int check_if_dead(t_vars *vars)
{
	int i = 0;
	long time_now;
	while(i < vars->nb_philo)
	{
		time_now = time_passed(vars->start_time);
		safe_mutex_lock(&vars->philosophers[i].mutex, vars);
		if(time_now - vars->philosophers[i].last_time_eat > vars->time_to_die)
		{
			printf("%ld %d is dead\n", time_now, vars->philosophers[i].id);
			safe_mutex_lock(&vars->mutex, vars);
			vars->stop_simulation = 1;
			safe_mutex_unlock(&vars->mutex, vars);
			safe_mutex_unlock(&vars->philosophers[i].mutex, vars);
			safe_mutex_unlock(&vars->philosophers[i].left_fork->mutex, vars);
			safe_mutex_unlock(&vars->philosophers[i].right_fork->mutex, vars);
			return (1);
		}
		safe_mutex_unlock(&vars->philosophers[i].mutex, vars);
		i++;
	}
	return (0);
}

void *monitor(void *vars)
{
	t_vars *tmp = (t_vars *)vars;
	while(1)
	{
		if(check_if_done(tmp))
		{
			safe_mutex_lock(&tmp->mutex, tmp);
			tmp->stop_simulation = 1;
			safe_mutex_unlock(&tmp->mutex, tmp);
			return (NULL);
		}
		if(check_if_dead(tmp))
			return (NULL);
	}
	return (NULL);
}

void clean_memory(t_vars *vars)
{
	int i = 0;
	while(i < vars->nb_philo)
	{
		if (pthread_mutex_destroy(&vars->philosophers[i].mutex) != 0)
		{
			write(2, "pthread_mutex_destroy() error\n", 30);
			return ;
		}
		if (pthread_mutex_destroy(&vars->forks[i].mutex) != 0)
		{
			write(2, "pthread_mutex_destroy() error\n", 30);
			return ;
		}
		i++;
	}
	if (pthread_mutex_destroy(&vars->mutex) != 0)
	{
		write(2, "pthread_mutex_destroy() error\n", 30);
		return ;
	}
	free(vars->philosophers);
	free(vars->forks);
}

int main(int argc, char **argv)
{
	// pthread_t thread;
	t_vars vars;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	// vars.start_time = tv.tv_sec;
	if(!check_argv(argc, argv) || argc > 6 || argc < 5 || ft_atoi(argv[1]) == 0)
	{
		printf("Invalid Parameters\n");
		return (1);
	}
	vars.nb_philo = ft_atoi(argv[1]);
	vars.time_to_die = ft_atoi(argv[2]);
	vars.time_to_eat = ft_atoi(argv[3]);
	vars.time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
	{
		if (ft_atoi(argv[5]) == 0)
			return (0);
		vars.nb_meals = ft_atoi(argv[5]);
	}
	else
		vars.nb_meals = -1;
	vars.start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	vars.stop_simulation = 0;
	if (!init_forks(&vars))
		return (1);
	if (!init_philos(&vars))
	{
		free(vars.forks);
		return (1);
	}
	if (pthread_mutex_init(&vars.mutex, NULL) != 0)
	{
		write(2, "pthread_mutex_init() error\n", 28);
		free(vars.forks);
		free(vars.philosophers);
		return (1);
	}
	int i = 0;
	while(i < vars.nb_philo)
	{
		if (pthread_create(&vars.philosophers[i].thread, NULL, &routine, &vars.philosophers[i]) != 0)
		{
			write(2, "pthread_create() error\n", 24);
			safe_mutex_lock(&vars.mutex, &vars);
			vars.stop_simulation = 1;
			safe_mutex_unlock(&vars.mutex, &vars);
			while(i >= 0)
			{
				if (pthread_join(vars.philosophers[i].thread, NULL) != 0)
				{
					write(2, "pthread_join() error\n", 22);
					clean_memory(&vars);
					return (1);
				}
				i--;
			}
			clean_memory(&vars);
			return (1);
		}
		usleep(300);
		i++;
	}
	if (pthread_create(&vars.monitor, NULL, &monitor, &vars) != 0)
	{
		write(2, "pthread_create() error\n", 24);
		clean_memory(&vars);
		return (1);
	}
	i = 0;
	while(i < vars.nb_philo)
	{
		
		if (pthread_join(vars.philosophers[i].thread, NULL) != 0)
		{
			write(2, "pthread_join() error\n", 22);
			clean_memory(&vars);
			return (1);
		}
		// printf("philo %d left_fork: %d | right_fork: %d\n", vars.philosophers[i].id, vars.philosophers[i].left_fork->id, vars.philosophers[i].right_fork->id);
		i++;
	}
	if (pthread_join(vars.monitor, NULL) != 0)
	{
		write(2, "pthread_join() error\n", 22);
		clean_memory(&vars);
		return (1);
	}
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
