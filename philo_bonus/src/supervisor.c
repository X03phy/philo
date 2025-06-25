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

// static bool	all_full_of_spaghetti(t_table *table)
// {
// 	int	i;

// 	if (table->nb_miam == -1)
// 		return (false);
// 	i = 0;
// 	while (i < table->nb_philos)
// 	{
// 		sem_wait(table->philos[i].meal_counter_lock);
// 		// printf("meal_count: %d and miam: %d\n", table->philos[i].meal_counter, table->nb_miam );
// 		if (table->philos[i].meal_counter < table->nb_miam)
// 		{
// 			sem_post(table->philos[i].meal_counter_lock);
// 			return (false);
// 		}
// 		sem_post(table->philos[i].meal_counter_lock);
// 		i++;
// 	}
// 	safe_print(&table->philos[0], FULLING);
// 	sem_wait(table->end_lock);
// 	table->end_simulation = true;
// 	sem_post(table->end_lock);
// 	return (true);
// }

static bool	this_is_the_end(t_philo *philo)
{
	time_t	time;

	sem_wait(philo->meal_time_lock);
	time = get_time_ms() - philo->last_meal_time;
	sem_post(philo->meal_time_lock);
	if (time >= philo->table->time_to_die)
		return (true);
	return (false);
}

void	*personal_supervisor(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	synchronize_all(philo->table->start_time);
	while (true)
	{
		if (this_is_the_end(philo) == true)
			break ;
		usleep(100);
	}
	exit(0);
	return (NULL);
}
