/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:59:49 by oait-laa          #+#    #+#             */
/*   Updated: 2024/04/21 19:57:35 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	fork_process(t_vars *vars, int *pid, int *i)
{
	printf("main process %d created\n", getpid());
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
			vars->philosophers[*i].pid = *pid;
			*pid = fork();
			if (*pid == -1)
			{
				write(2, "fork() error\n", 13);
				return (1);
			}
		}
		else
			break ;
		(*i)++;
	}
	if (*pid != 0)
		vars->philosophers[*i].pid = *pid;
		
	// printf("child process %d created\n", vars->philosophers[*i].pid);
	return (0);
}

int	clean_mem(t_vars *vars, int pid)
{
	if (sem_close(vars->forks) != 0)
		return (1);
	if (sem_close(vars->stop) != 0)
		return (1);
	if (sem_close(vars->print) != 0)
		return (1);
	if (sem_close(vars->eat) != 0)
		return (1);
	if (sem_close(vars->eat_time) != 0)
		return (1);
	if (sem_close(vars->death) != 0)
		return (1);
	if (pid != 0)
	{
		sem_unlink("forks");
		sem_unlink("stop");
		sem_unlink("print");
		sem_unlink("eat");
		sem_unlink("eat_time");
		sem_unlink("death");
	}
	free(vars->philosophers);
	return (0);
}

int	join_threads(t_vars *vars, int i, int pid)
{
	if (pthread_join(vars->death_thread, NULL) != 0)
	{
		write(2, "pthread_join() error\n", 21);
		safe_sem_post(vars->death, &vars->philosophers[i]);
		clean_mem(vars, pid);
		return (1);
	}
	if (pthread_join(vars->monitor_thread, NULL) != 0)
	{
		write(2, "pthread_join() error\n", 21);
		safe_sem_post(vars->death, &vars->philosophers[i]);
		clean_mem(vars, pid);
		return (1);
	}
	return (0);
}

int	start_simulation(t_vars *vars, int *i, int *pid)
{
	int	status;

	
	if (fork_process(vars, pid, i))
	{
		if (clean_mem(vars, *pid))
			write(2, "sem_close() error\n", 18);
		return (1);
	}
	if (*pid == 0)
	{
		if (init_threads(vars, *i, *pid))
			return (1);
		routine(&vars->philosophers[*i]);
		if (join_threads(vars, *i, *pid))
			return (1);
	}
	while (waitpid(-1, &status, 0) > 0)
	{
		*pid = waitpid(-1, &status, 0);
		if (*pid == -1 && vars->nb_philo == 1)
		{
			print_msg(&vars->philosophers[*i], "died");
			break ;
		}
		while (*i >= 0)
		{
			// printf("exit status %d\n", WEXITSTATUS(status));
			if (vars->philosophers[*i].pid == *pid && WEXITSTATUS(status) == 2)
			{
				print_msg(&vars->philosophers[*i], "died");
				break ;
			}
			(*i)--;
		}
	}
	if (clean_mem(vars, *pid))
	{
		write(2, "sem_close() error\n", 18);
		return (1);
	}
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
		if (clean_mem(&vars, pid))
			write(2, "sem_close() error\n", 18);
		return (1);
	}
	if (start_simulation(&vars, &i, &pid))
		return (1);
	return (0);
}
