/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:26:26 by ebonutto          #+#    #+#             */
/*   Updated: 2025/06/20 15:43:26 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	all_full_of_spaghetti(t_table *table)
{
	int	i;

	if (table->nb_miam == -1)
		return (false);
	i = 0;
	while (i < table->nb_philos)
	{
		sem_wait(table->philos[i].meal_counter_lock);
		// printf("meal_count: %d and miam: %d\n", table->philos[i].meal_counter, table->nb_miam );
		if (table->philos[i].meal_counter < table->nb_miam)
		{
			sem_post(table->philos[i].meal_counter_lock);
			return (false);
		}
		sem_post(table->philos[i].meal_counter_lock);
		i++;
	}
	safe_print(&table->philos[0], FULLING);
	sem_wait(table->end_lock);
	table->end_simulation = true;
	sem_post(table->end_lock);
	return (true);
}

static bool	this_is_the_end(t_table *table)
{
	int		i;
	time_t	time;

	if (all_full_of_spaghetti(table) == true)
		return (true);
	i = 0;
	while (i < table->nb_philos)
	{
		sem_wait(table->philos[i].meal_time_lock);
		time = get_time_ms() - table->philos[i].last_meal_time;
		sem_post(table->philos[i].meal_time_lock);
		if (time >= table->time_to_die)
		{
			safe_print(&table->philos[i], DYING);
			sem_wait(table->end_lock);
			table->end_simulation = true;
			sem_post(table->end_lock);
			return (true);
		}
		i++;
	}
	return (false);
}

void	*supervisor(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	if (table->nb_philos == 1)
		return (NULL);
	synchronize_all(table->start_time);
	while (true)
	{
		if (this_is_the_end(table) == true)
			break ;
		usleep(100);
	}
	return (NULL);
}
