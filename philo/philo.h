/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 11:42:24 by oait-laa          #+#    #+#             */
/*   Updated: 2024/02/19 14:33:28 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_vars	t_vars;

typedef struct s_fork
{
	int				id;
	pthread_mutex_t	mutex;
}	t_fork;

typedef struct s_philosopher
{
	int				id;
	long			last_time_eat;
	t_fork			*left_fork;
	t_fork			*right_fork;
	int				nb_meals;
	pthread_t		thread;
	pthread_mutex_t	mutex;
	t_vars			*vars;
}	t_philosopher;

typedef struct s_vars
{
	int				nb_philo;
	long			start_time;
	int				stop_simulation;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_meals;
	t_philosopher	*philosophers;
	pthread_t		monitor;
	pthread_mutex_t	mutex;
	t_fork			*forks;
}	t_vars;

long	ft_atoi(const char *str);
int		check_argv(int argc, char **argv);
int		init_mutex(t_vars *vars, pthread_mutex_t *mutex, int philo, int forks);
int		init_philos(t_vars *vars);
int		init_forks(t_vars *vars);
int		init_values(t_vars *vars, int argc, char **argv);
int		handle_create_thread_error(t_vars *vars, int i);
void	clean_memory(t_vars *vars);
void	destroy_forks_mutex(t_vars *vars, int nb_philo);
void	destroy_philo_mutex(t_vars *vars, int nb_philo);
int		join_threads(t_vars *vars);
int		start_threads(t_vars *vars);
int		start_monitor(t_vars *vars, int i);
int		safe_thread_join(pthread_t thread, t_vars *vars);
void	*monitor(void *vars);
int		stop_similation(t_philosopher *philo);
int		check_if_dead(t_vars *vars);
int		check_if_done(t_vars *vars);
int		lock_forks(t_philosopher *philo);
int		eat_routine(t_philosopher *philo);
int		sleep_routine(t_philosopher *philo);
int		think_routine(t_philosopher *philo);
void	*routine(void *philo);
long	time_now(t_vars *vars);
void	accurate_usleep(long m_sec, t_vars *vars);
long	time_passed(t_vars *vars);
void	safe_mutex_unlock(pthread_mutex_t *mutex, t_vars *vars);
void	safe_mutex_lock(pthread_mutex_t *mutex, t_vars *vars);
void	print_msg(t_philosopher *philo, char *msg);

#endif