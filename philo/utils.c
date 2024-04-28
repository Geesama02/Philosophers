/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 14:30:42 by oait-laa          #+#    #+#             */
/*   Updated: 2024/04/28 14:32:01 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_msg(t_philosopher *philo, char *msg)
{
	pthread_mutex_lock(&philo->vars->mutex);
	if (philo->vars->stop_simulation == 0)
		printf("%ld %d %s\n", time_passed(philo->vars), philo->id, msg);
	pthread_mutex_unlock(&philo->vars->mutex);
}
