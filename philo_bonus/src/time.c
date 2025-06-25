/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:58:47 by ebonutto          #+#    #+#             */
/*   Updated: 2025/06/20 15:07:35 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

time_t	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

// bool	is_this_the_end(t_table *table)
// {
// 	bool	is_end;

// 	is_end = false;
// 	sem_wait(table->end_lock);
// 	if (table->end_simulation == true)
// 		is_end = true;
// 	sem_post(table->end_lock);
// 	return (is_end);
// }

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
