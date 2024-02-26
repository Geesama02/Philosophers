/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:38:05 by oait-laa          #+#    #+#             */
/*   Updated: 2024/02/26 15:47:23 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <semaphore.h>
# include <signal.h>

typedef struct s_vars	t_vars;

typedef struct s_philosopher
{
	int				id;
	int				nb_must_eat;
	int				forks;
	int				last_time_eat;
	int				nb_meals;
	pthread_t		thread;
	t_vars			*vars;
}					t_philosopher;

typedef struct s_vars
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_meals;
	int				last_time_eat;
	long			start_time;
	int				stop_simulation;
	int				print_death;
	pthread_t		monitor_thread;
	pthread_t		death_thread;
	sem_t			*stop;
	sem_t			*eat_time;
	sem_t			*eat;
	sem_t			*death;
	sem_t			*print;
	t_philosopher	*philosophers;
	sem_t			*forks;
}					t_vars;

long	ft_atoi(const char *str);
int		check_argv(int argc, char **argv);
void	*routine(void *philo);
void	*monitor(void *vars);
int		stop_similation(t_philosopher *philo);
void	accurate_usleep(long m_sec, t_vars *vars);
int		stop_similation(t_philosopher *philo);
void	print_msg(t_philosopher *philo, char *msg);
long	time_passed(t_vars *vars);
int		check_if_done(t_philosopher *philo);
int		check_if_dead(t_philosopher *philo);
void	safe_sem_wait(sem_t *sem, t_philosopher *philo);
void	safe_sem_post(sem_t *sem, t_philosopher *philo);
int		init_values(t_vars *vars, int argc, char **argv);
int		init_philos(t_vars *vars);
int		init_semaphores(t_vars *vars);
int		fill_philo(t_vars *vars);
void	*monitor_death(void *vars);
int		init_threads(t_vars *vars, int i, int pid);
int		clean_mem(t_vars *vars, int pid);

#endif