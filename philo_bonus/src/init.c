/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:58:34 by ebonutto          #+#    #+#             */
/*   Updated: 2025/06/27 17:34:38 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_forks(t_table *table)
{
	sem_unlink("/fork");
	table->forks = sem_open("/fork", O_CREAT | O_EXCL, 0644, table->nb_philos);
	if (table->forks == SEM_FAILED)
		return (ERROR_CODE);
	return (0);
}

static int	close_breautifully(t_table *table, char *name, int i, bool second)
{
	sem_close(table->forks);
	sem_unlink("/fork");
	if (second)
	{
		sem_close(table->philos[i].meal_time_lock);
		get_name(name, "pmtl", i);
		sem_unlink(name);
	}
	while (--i >= 0)
	{
		sem_close(table->philos[i].meal_time_lock);
		get_name(name, "pmtl", i);
		sem_unlink(name);
		sem_close(table->philos[i].meal_counter_lock);
		get_name(name, "pmcl", i);
		sem_unlink(name);
	}
	return (ERROR_CODE);
}

static int	init_forks_and_philos(t_table *table)
{
	char	name[10];
	int		i;

	if (init_forks(table) != 0)
		return (ERROR_CODE);
	i = -1;
	while (++i < table->nb_philos)
	{
		memset(&table->philos[i], 0, sizeof(t_philo));
		table->philos[i].table = table;
		table->philos[i].philo_id = i + 1;
		get_name(name, "pmtl", i);
		sem_unlink(name);
		table->philos[i].meal_time_lock = sem_open(name,
				O_CREAT | O_EXCL, 0644, 1);
		if (table->philos[i].meal_time_lock == SEM_FAILED)
			return(close_breautifully(table, name, i, false));
		get_name(name, "pmcl", i);
		sem_unlink(name);
		table->philos[i].meal_counter_lock = sem_open(name,
				O_CREAT | O_EXCL, 0644, 1);
		if (table->philos[i].meal_counter_lock == SEM_FAILED)
			return (close_breautifully(table, name, i, true));
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
	memset(table, 0, sizeof(t_table));
	sem_unlink("/end_lock");
	table->death_lock = sem_open("/end_lock", O_CREAT | O_EXCL, 0644, 1);      // securiser ici
	if (parse_input(argc, argv, table) != 0)
		return (1);
	if (table->nb_philos == 0)
		return (0);
	sem_unlink("/write_lock");
	table->write_lock = sem_open("/write_lock", O_CREAT | O_EXCL, 0644, 1);
	if (table->write_lock == SEM_FAILED)
		return (perror("sem_open(): write_lock"), ERROR_CODE);
	sem_unlink("/miam_lock");
	table->miam_lock = sem_open("/miam_lock", O_CREAT | O_EXCL, 0644, 0);
	if (table->miam_lock == SEM_FAILED)
	{
		sem_close(table->write_lock);
		sem_unlink("/write_lock");
		return (perror("sem_open(): miam_lock"), ERROR_CODE);
	}
	if (init_forks_and_philos(table) != 0)
	{
		sem_close(table->write_lock);
		sem_unlink("/write_lock");
		sem_close(table->miam_lock);
		sem_unlink("/miam_lock");
		return (ERROR_CODE);
	}
	return (0);
}
