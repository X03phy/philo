/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:58:38 by ebonutto          #+#    #+#             */
/*   Updated: 2025/06/21 17:32:27 by ebonutto         ###   ########.fr       */
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
	// int	i;

	// i = 0;
	// while (i < table->nb_philos)
	// {
	// 	sem_close(table->forks[i]);
	// 	sem_close(table->philos[i].meal_time_lock);
	// 	i++;
	// }
	// free(table->forks);
	// free(table->philos);
	// sem_close(&table->write_lock);
	// sem_close(&table->end_lock);
	(void)table;
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
