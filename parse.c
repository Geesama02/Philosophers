/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 11:12:57 by oait-laa          #+#    #+#             */
/*   Updated: 2024/02/18 12:42:32 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	found_num(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			return (1);
		i++;
	}
	return (0);
}

static int	is_num(char *str)
{
	int	i;

	i = 0;
	if (!found_num(str))
		return (0);
	while (str[i] == ' ')
		i++;
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		if ((str[i] < '0' || str[i] > '9') && str[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

static int	count_digits(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] && (str[i] == '0' || str[i] == ' ' || str[i] == '+'))
		i++;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		i++;
		j++;
	}
	return (j);
}

int check_argv(int argc, char **argv)
{
	int i;

	i = 1;
	while (i < argc)
	{
		if (count_digits(argv[i]) > 11 || ft_atoi(argv[i]) > 2147483647
			|| ft_atoi(argv[i]) <= 0 || !is_num(argv[i]))
			return (0);
		i++;
	}
	return (1);
}
