/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:59:49 by oait-laa          #+#    #+#             */
/*   Updated: 2024/04/29 10:23:16 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	fork_process(t_vars *vars, int *pid, int *i)
{
	*pid = fork();
	if (*pid == -1)
	{
		write(2, "fork() error\n", 13);
		return (1);
	}
	while (*i < vars->nb_philo - 1)
	{
		usleep(300);
		if (*pid != 0)
		{
			*pid = fork();
			if (*pid == -1)
			{
				write(2, "fork() error\n", 13);
				return (1);
			}
		}
		else
			break ;
		vars->philosophers[*i].pid = *pid;
		(*i)++;
	}
	return (0);
}

int	clean_mem(t_vars *vars, int pid)
{
	free(vars->philosophers);
	if (sem_close(vars->forks) != 0)
		write(2, "sem_close() error\n", 18);
	if (sem_close(vars->stop) != 0)
		write(2, "sem_close() error\n", 18);
	if (sem_close(vars->print) != 0)
		write(2, "sem_close() error\n", 18);
	if (sem_close(vars->eat) != 0)
		write(2, "sem_close() error\n", 18);
	if (sem_close(vars->eat_time) != 0)
		write(2, "sem_close() error\n", 18);
	if (sem_close(vars->death) != 0)
		write(2, "sem_close() error\n", 18);
	if (pid != 0)
		unlink_sem();
	return (0);
}

int	detach_threads(t_vars *vars, int pid)
{
	if (pthread_detach(vars->death_thread) != 0)
	{
		write(2, "pthread_detach() error\n", 21);
		sem_post(vars->death);
		clean_mem(vars, pid);
		return (1);
	}
	if (pthread_detach(vars->monitor_thread) != 0)
	{
		write(2, "pthread_detach() error\n", 21);
		sem_post(vars->death);
		clean_mem(vars, pid);
		return (1);
	}
	return (0);
}

int	start_simulation(t_vars *vars, int *i, int *pid)
{
	if (fork_process(vars, pid, i))
	{
		clean_mem(vars, *pid);
		return (1);
	}
	if (*pid == 0)
	{
		if (init_threads(vars, *i, *pid))
			return (1);
		routine(&vars->philosophers[*i]);
		if (detach_threads(vars, *pid))
			return (1);
	}
	wait_processs(vars, i);
	clean_mem(vars, *pid);
	return (0);
}

int	main(int argc, char **argv)
{
	t_vars	vars;
	int		i;
	int		pid;

	i = 0;
	pid = 0;
	if (!check_argv(argc, argv) || argc > 6 || argc < 5)
	{
		printf("Invalid Parameters\n");
		return (1);
	}
	if (init_values(&vars, argc, argv))
		return (0);
	if (!init_philos(&vars))
		return (1);
	if (init_semaphores(&vars))
	{
		write(2, "sem_open() error\n", 17);
		clean_mem(&vars, 1);
		return (1);
	}
	if (start_simulation(&vars, &i, &pid))
		return (1);
	return (0);
}
