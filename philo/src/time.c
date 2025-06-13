/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:58:47 by ebonutto          #+#    #+#             */
/*   Updated: 2025/03/13 19:05:17 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

time_t	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

bool	is_this_the_end(t_table *table)
{
	bool	is_end;

	is_end = false;
	pthread_mutex_lock(&table->end_lock);
	if (table->end_simulation == true)
		is_end = true;
	pthread_mutex_unlock(&table->end_lock);
	return (is_end);
}

void	philo_sleep_check(t_philo *philo, time_t sleep_time)
{
	time_t	alarm;

	alarm = get_time_ms() + sleep_time;
	while (get_time_ms() < alarm)
	{
		if (is_this_the_end(philo->table) == true)
			return ;
		usleep(100);
	}
}

void	synchronize_all(time_t start)
{
	while (get_time_ms() < start)
		;
}
