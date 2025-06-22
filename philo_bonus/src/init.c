/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:58:34 by ebonutto          #+#    #+#             */
/*   Updated: 2025/06/21 17:35:32 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
void get_name( char *name, char *what, int i )
{
	int		n;

	if (i < 10)
		n = 1;
	else if (i < 100)
		n = 2;
	else
		n = 3;
	name[0] = '/';
	name[1] = what[0];
	name[2] = what[1];
	name[3] = what[2];
	name[4] = what[3];
	name[5] = '_';
	name[5 + n + 1] = '\0';
	while ( n > 0 )
	{
		name[5 + n] = (i % 10) + '0';
		i /= 10;
		--n;
	}
}

static int	init_forks(t_table *table)
{
	int	i;
	char name[10];

	table->forks = malloc(sizeof(sem_t *) * table->nb_philos);
	if (!table->forks)
	{
		perror("malloc()");
		return (ERROR_CODE);
	}
	i = 0;
	while (i < table->nb_philos)
	{
		get_name( name, "fork", i );
		sem_unlink(name);
		table->forks[i] = sem_open(name, O_CREAT | O_EXCL, 0644, 1);
		if (table->forks[i] == SEM_FAILED)
		{
			perror("sem_open()");
			while (--i >= 0)
			{
				get_name( name, "fork", i );
				sem_close(table->forks[i]);
				sem_unlink(name);
			}
			free(table->forks);
			return (ERROR_CODE);
		}
		i++;
	}
	return (0);
}

// static int	secure_fail_philos(t_table *table, bool destroy_mtl,
// 		int i, bool malloc)
// {
// 	(void)malloc;
// 	(void)table;
// 	(void)destroy_mtl;
// 	(void)i;
// 	// if (malloc == true)
// 	// {
// 	// 	perror("malloc()");
// 	// 	while (++i < table->nb_philos)
// 	// 		pthread_mutex_destroy(&(table->forks[i].fork));
// 	// 	free(table->forks);
// 	// 	return (ERROR_CODE);
// 	// }
// 	// perror("pthread_mutex_init()");
// 	// if (destroy_mtl == true)
// 	// 	pthread_mutex_destroy(&(table->philos[i].meal_time_lock));
// 	// while (0 < i--)
// 	// {
// 	// 	pthread_mutex_destroy(&(table->philos[i].meal_time_lock));
// 	// 	pthread_mutex_destroy(&(table->philos[i].meal_counter_lock));
// 	// }
// 	// free(table->philos);
// 	// i = -1;
// 	// while (++i < table->nb_philos)
// 	// 	pthread_mutex_destroy(&table->forks[i].fork);
// 	// free(table->forks);
// 	return (ERROR_CODE);
// }

static int	init_philos(t_table *table)
{
	char	name[10];
	int	i;

	i = -1;
	table->philos = malloc(sizeof(t_philo) * table->nb_philos);
	if (!table->philos)
		return (ERROR_CODE);
	while (++i < table->nb_philos)
	{
		memset(&table->philos[i], 0, sizeof(t_philo));
		table->philos[i].table = table;
		table->philos[i].philo_id = i + 1;

		get_name(name, "pmtl", i);
		sem_unlink(name);
		table->philos[i].meal_time_lock = sem_open(name, O_CREAT | O_EXCL, 0644, 1);
		// if (table->philos[i].meal_time_lock == SEM_FAILED)

		get_name(name, "pmcl", i);
		sem_unlink(name);
		table->philos[i].meal_counter_lock = sem_open(name, O_CREAT | O_EXCL, 0644, 1);
		// if (table->philos[i].meal_counter_lock == SEM_FAILED)

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
	sem_unlink("/write_lock");
	table->write_lock = sem_open("/write_lock", O_CREAT | O_EXCL, 0644, 1);
	if (table->write_lock == SEM_FAILED)
		return (perror("sem_open(): write_lock"), ERROR_CODE);
	sem_unlink("/end_lock");
	table->end_lock = sem_open("/end_lock", O_CREAT | O_EXCL, 0644, 1);
	if (table->end_lock == SEM_FAILED)
	{
		sem_close(table->write_lock);
		sem_unlink("/write_lock");
		return (perror("sem_open(): end_lock"), ERROR_CODE);
	}
	if (init_forks(table) != 0 || init_philos(table) != 0)
	{
		sem_close(table->write_lock);
		sem_unlink("/write_lock");
		sem_close(table->end_lock);
		sem_unlink("/end_lock");
		return (ERROR_CODE);
	}
	return (0);
}
