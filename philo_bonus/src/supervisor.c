/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:26:26 by ebonutto          #+#    #+#             */
/*   Updated: 2025/06/27 17:38:58 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*global_supervisor_death(void *arg)
{
	t_philo	*arg;
	if (  )
}

void	*personal_supervisor(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	synchronize_all(philo->table->start_time);
	while (true)
	{
		if (this_is_the_end(philo) == true)
			break;
		usleep(100);
	}
	return (NULL);
}

void	kill_them_all(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		kill(table->pids[i], SIGKILL);
		++i;
	}
}

void	*global_famine_supervisor(void *arg)
{
	t_table	*table;
	time_t	time;
	int		i;

	table = (t_table *)arg;
	synchronize_all(table->start_time);
	i = 0;
	while (i < table->nb_philos)
	{
		sem_wait(table->miam_lock);
		++i;
	}
	// kill_them_all(table);
	time = get_time_ms() - table->start_time;
	printf("%ldms All philosophers finished eating ⭐\n", time);
	return (NULL);
}
