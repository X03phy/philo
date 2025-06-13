/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:58:43 by ebonutto          #+#    #+#             */
/*   Updated: 2025/03/15 14:57:57 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*philosophers(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal_time = philo->table->start_time;
	pthread_mutex_unlock(&philo->meal_time_lock);
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
	return (NULL);
}

static int	remi_sans_famille(t_table *table)
{
	table->start_time = get_time_ms();
	pthread_mutex_lock(&(table->philos[0].left_fork->fork));
	safe_print(&(table->philos[0]), FORKING);
	philo_sleep_check(&(table->philos[0]), table->time_to_die);
	safe_print(&(table->philos[0]), DYING);
	pthread_mutex_unlock(&(table->philos[0].left_fork->fork));
	return (0);
}

int	dinner(t_table *table)
{
	int	i;

	if (table->nb_miam == 0)
		return (0);
	if (table->nb_philos == 1)
		return (remi_sans_famille(table));
	table->start_time = get_time_ms() + (table->nb_philos * 20);
	i = -1;
	while (++i < table->nb_philos)
	{
		if (pthread_create(&table->philos[i].philo, NULL, &philosophers,
				&(table->philos[i])) != 0)
			return (perror("pthread_create()"), ERROR_CODE);
	}
	if (pthread_create(&table->supervisor, NULL, &supervisor, table) != 0)
		return (perror("pthread_create()"), ERROR_CODE);
	i = -1;
	while (++i < table->nb_philos)
		pthread_join(table->philos[i].philo, NULL);
	pthread_join(table->supervisor, NULL);
	return (0);
}
