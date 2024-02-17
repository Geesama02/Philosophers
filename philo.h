/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 11:42:24 by oait-laa          #+#    #+#             */
/*   Updated: 2024/02/17 15:56:30 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_vars t_vars;

typedef struct s_fork
{
	int id;
	pthread_mutex_t	mutex;
}	t_fork;

typedef struct s_philosopher
{
	int id;
	long last_time_eat;
	t_fork *left_fork;
	t_fork *right_fork;
	int nb_meals;
	pthread_t	thread;
	pthread_mutex_t	mutex;
	t_vars	*vars;
}	t_philosopher;

typedef struct s_vars
{
	int nb_philo;
	long start_time;
	int stop_simulation;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int nb_meals;
	t_philosopher *philosophers;
	pthread_t monitor;
	pthread_mutex_t	mutex;
	t_fork *forks;
}	t_vars;

int	ft_atoi(const char *str);

#endif