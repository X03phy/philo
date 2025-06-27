/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:58:47 by ebonutto          #+#    #+#             */
/*   Updated: 2025/06/27 17:35:44 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

time_t	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	philo_sleep_check(t_philo *philo, time_t sleep_time)
{
	time_t	alarm;
	(void)philo;

	alarm = get_time_ms() + sleep_time;
	while (get_time_ms() < alarm)
	{
		usleep(100);
	}
}

void	synchronize_all(time_t start)
{
	while (get_time_ms() < start)
		;
}

bool	this_is_the_end(t_philo *philo)
{
	time_t	time;

	sem_wait(philo->meal_time_lock);
	time = get_time_ms() - philo->last_meal_time;
	sem_post(philo->meal_time_lock);
	sem_wait(philo->meal_counter_lock);
	if (philo->done_eating == false && philo->table->nb_miam != -1 && philo->meal_counter >= philo->table->nb_miam)
	{
		philo->done_eating = true;
		sem_post(philo->table->miam_lock);
	}
	sem_post(philo->meal_counter_lock);
	if (time >= philo->table->time_to_die)
	{
		sem_wait(philo->table->death_lock);
		philo->table->death = true;
		sem_post(philo->table->death_lock);
		return (true);
	}
}

bool	is_this_the_end(t_table *table)
{
	sem_wait(table->death_lock);
	if (table->death)
	{
		sem_post(table->death_lock);
		return (true);
	}
	return (false);
}
