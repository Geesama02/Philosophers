/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 13:09:14 by oait-laa          #+#    #+#             */
/*   Updated: 2024/02/25 13:16:30 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	safe_sem_wait(sem_t *sem, t_philosopher *philo)
{
	if (sem_wait(sem) != 0)
	{
		write(2, "sem_wait() error\n", 17);
		clean_mem(philo->vars);
		exit(1);
	}
}

void	safe_sem_post(sem_t *sem, t_philosopher *philo)
{
	if (sem_post(sem) != 0)
	{
		write(2, "sem_post() error\n", 17);
		clean_mem(philo->vars);
		exit(1);
	}
}