/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:26:26 by ebonutto          #+#    #+#             */
/*   Updated: 2025/03/15 14:40:54 by x03phy           ###   ########.fr       */
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
		pthread_mutex_lock(&table->philos[i].meal_counter_lock);
		if (table->philos[i].meal_counter < table->nb_miam)
		{
			pthread_mutex_unlock(&table->philos[i].meal_counter_lock);
			return (false);
		}
		pthread_mutex_unlock(&table->philos[i].meal_counter_lock);
		i++;
	}
	safe_print(&table->philos[0], FULLING);
	pthread_mutex_lock(&table->end_lock);
	table->end_simulation = true;
	pthread_mutex_unlock(&table->end_lock);
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
		pthread_mutex_lock(&table->philos[i].meal_time_lock);
		time = get_time_ms() - table->philos[i].last_meal_time;
		pthread_mutex_unlock(&table->philos[i].meal_time_lock);
		if (time >= table->time_to_die)
		{
			safe_print(&table->philos[i], DYING);
			pthread_mutex_lock(&table->end_lock);
			table->end_simulation = true;
			pthread_mutex_unlock(&table->end_lock);
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
