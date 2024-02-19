/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:54:59 by oait-laa          #+#    #+#             */
/*   Updated: 2024/02/19 14:40:09 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_vars	vars;

	if (!check_argv(argc, argv) || argc > 6 || argc < 5)
	{
		printf("Invalid Parameters\n");
		return (1);
	}
	if (init_values(&vars, argc, argv))
		return (0);
	if (!init_forks(&vars))
		return (1);
	if (!init_philos(&vars))
		return (1);
	if (init_mutex(&vars, &vars.mutex, vars.nb_philo, vars.nb_philo))
		return (1);
	if (start_threads(&vars))
		return (1);
	if (join_threads(&vars))
		return (1);
	clean_memory(&vars);
	return (0);
}
