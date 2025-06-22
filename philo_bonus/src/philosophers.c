/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:58:43 by ebonutto          #+#    #+#             */
/*   Updated: 2025/06/20 15:48:53 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philosophers(t_philo *philo)
{
	sem_wait(philo->meal_time_lock);
	philo->last_meal_time = philo->table->start_time;
	sem_post(philo->meal_time_lock);
	synchronize_all(philo->table->start_time);
	if (philo->philo_id % 2 == 1)
	{
		safe_print(philo, THINKING);
		philo_sleep_check(philo, philo->table->time_to_eat - 100);
	}
	while (true)
	{
		philo_eat(philo);
		if (is_this_the_end(philo->table) == true)
			break ;
		philo_sleep(philo);
		if (is_this_the_end(philo->table) == true)
			break ;
		philo_think(philo);
		if (is_this_the_end(philo->table) == true)
			break ;
	}
}

static int	remi_sans_famille(t_table *table)
{
	table->start_time = get_time_ms();
	sem_wait(table->forks[0]);
	safe_print(&(table->philos[0]), FORKING);
	philo_sleep_check(&(table->philos[0]), table->time_to_die);
	safe_print(&(table->philos[0]), DYING);
	sem_post(table->forks[0]);
	return (0);
}

int	dinner(t_table *table)
{
	int	i;
	pid_t pid;

	if (table->nb_miam == 0)
		return (0);
	if (table->nb_philos == 1)
		return (remi_sans_famille(table));
	table->start_time = get_time_ms() + (table->nb_philos * 20);
	i = -1;
	while (++i < table->nb_philos)
	{
		table->philos[i].last_meal_time = table->start_time;
		pid = fork();
		if (pid < 0)	
			return (perror("fork()"), ERROR_CODE); //  gerer le pb ici.
		if (pid == 0)
		{	
			philosophers(&(table->philos[i]));
			exit(0);
		}
	}
	if (pthread_create(&table->supervisor, NULL, &supervisor, table) != 0)
		return (perror("pthread_create()"), ERROR_CODE);
	i = -1;
	while (++i < table->nb_philos)
		waitpid(-1, NULL, 0);
	pthread_join(table->supervisor, NULL);
	return (0);
}
