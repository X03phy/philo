/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:58:38 by ebonutto          #+#    #+#             */
/*   Updated: 2025/03/14 14:58:18 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	error_message(char *message1, char *message2)
{
	int	i;

	i = 0;
	while (message1[i])
	{
		write(2, &message1[i], 1);
		i++;
	}
	i = 0;
	while (message2[i])
	{
		write(2, &message2[i], 1);
		i++;
	}
	return (1);
}

static void	free_philo(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		pthread_mutex_destroy(&(table->forks[i].fork));
		pthread_mutex_destroy(&(table->philos[i].meal_time_lock));
		i++;
	}
	free(table->forks);
	free(table->philos);
	pthread_mutex_destroy(&table->write_lock);
	pthread_mutex_destroy(&table->end_lock);
}

int	main(int argc, char **argv)
{
	t_table	table;
	int		error_code;

	if (argc == 5 || argc == 6)
	{
		error_code = init_table(argc, argv, &table);
		if (error_code != 0)
			return (error_code);
		error_code = dinner(&table);
		free_philo(&table);
		return (error_code);
	}
	error_message("Error: ",
		"The program is expecting a total of 4 or 5 arguments\n");
	return (1);
}
