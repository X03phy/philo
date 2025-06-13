/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:58:34 by ebonutto          #+#    #+#             */
/*   Updated: 2025/03/15 15:09:04 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_forks(t_table *table)
{
	int	i;

	table->forks = malloc(sizeof(t_fork) * table->nb_philos);
	if (!table->forks)
	{
		perror("malloc()");
		return (ERROR_CODE);
	}
	i = 0;
	while (i < table->nb_philos)
	{
		table->forks[i].fork_id = i;
		if (pthread_mutex_init(&((table->forks[i]).fork), NULL) != 0)
		{
			perror("pthread_mutex_init()");
			while (0 < i--)
				pthread_mutex_destroy(&(table->forks[i].fork));
			free(table->forks);
			return (ERROR_CODE);
		}
		i++;
	}
	return (0);
}

static int	secure_fail_philos(t_table *table, bool destroy_mtl,
		int i, bool malloc)
{
	if (malloc == true)
	{
		perror("malloc()");
		while (++i < table->nb_philos)
			pthread_mutex_destroy(&(table->forks[i].fork));
		free(table->forks);
		return (ERROR_CODE);
	}
	perror("pthread_mutex_init()");
	if (destroy_mtl == true)
		pthread_mutex_destroy(&(table->philos[i].meal_time_lock));
	while (0 < i--)
	{
		pthread_mutex_destroy(&(table->philos[i].meal_time_lock));
		pthread_mutex_destroy(&(table->philos[i].meal_counter_lock));
	}
	free(table->philos);
	i = -1;
	while (++i < table->nb_philos)
		pthread_mutex_destroy(&table->forks[i].fork);
	free(table->forks);
	return (ERROR_CODE);
}

static int	init_philos(t_table *table)
{
	int	i;

	i = -1;
	table->philos = malloc(sizeof(t_philo) * table->nb_philos);
	if (!table->philos)
		return (secure_fail_philos(table, false, i, true));
	while (++i < table->nb_philos)
	{
		memset(&table->philos[i], 0, sizeof(t_philo));
		table->philos[i].table = table;
		table->philos[i].philo_id = i + 1;
		table->philos[i].left_fork = &(table->forks[i]);
		if (table->nb_philos > 1)
			table->philos[i].right_fork = &(table->forks[(i + 1)
					% table->nb_philos]);
		if (pthread_mutex_init(&(table->philos[i].meal_time_lock), NULL) != 0)
			return (secure_fail_philos(table, false, i, false));
		if (pthread_mutex_init(&(table->philos[i].meal_counter_lock),
				NULL) != 0)
			return (secure_fail_philos(table, false, i, false));
	}
	return (0);
}

static int	parse_input(int argc, char **argv, t_table *table)
{
	table->nb_philos = atoi_check(argv[1]);
	if (table->nb_philos == -1 || table->nb_philos == 0
		|| table->nb_philos > 250)
		return (error_message(argv[1], ": is an invalid argument\n"));
	table->time_to_die = atoi_check(argv[2]);
	if (table->time_to_die == -1 || table->time_to_die == 0)
		return (error_message(argv[2], ": is an invalid argument\n"));
	table->time_to_eat = atoi_check(argv[3]);
	if (table->time_to_eat == -1 || table->time_to_eat == 0)
		return (error_message(argv[3], ": is an invalid argument\n"));
	table->time_to_sleep = atoi_check(argv[4]);
	if (table->time_to_sleep == -1 || table->time_to_sleep == 0)
		return (error_message(argv[4], ": is an invalid argument\n"));
	if (argc == 6)
	{
		table->nb_miam = atoi_check(argv[5]);
		if (table->nb_miam == -1)
			return (error_message(argv[5], ": is an invalid argument\n"));
	}
	else
		table->nb_miam = -1;
	return (0);
}

int	init_table(int argc, char **argv, t_table *table)
{
	if (parse_input(argc, argv, table) != 0)
		return (1);
	if (table->nb_philos == 0)
		return (0);
	if (pthread_mutex_init(&(table->write_lock), NULL) != 0)
		return (ERROR_CODE);
	if (pthread_mutex_init(&(table->end_lock), NULL) != 0)
	{
		pthread_mutex_destroy(&table->write_lock);
		return (ERROR_CODE);
	}
	table->end_simulation = false;
	table->everything_is_ready = false;
	if (init_forks(table) != 0 || init_philos(table) != 0)
	{
		pthread_mutex_destroy(&table->write_lock);
		pthread_mutex_destroy(&table->end_lock);
		return (ERROR_CODE);
	}
	return (0);
}
