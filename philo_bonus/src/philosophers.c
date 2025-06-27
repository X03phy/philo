/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:58:43 by ebonutto          #+#    #+#             */
/*   Updated: 2025/06/26 14:24:26 by ebonutto         ###   ########.fr       */
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
		philo_sleep(philo);
		philo_think(philo);
	}
}

static int	remi_sans_famille(t_table *table)
{
	table->start_time = get_time_ms();
	sem_wait(table->forks);
	safe_print(&(table->philos[0]), FORKING);
	philo_sleep_check(&(table->philos[0]), table->time_to_die);
	safe_print(&(table->philos[0]), DYING);
	sem_post(table->forks);
	return (0);
}

int	dinner(t_table *table)
{
	int nb;
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
			if (pthread_create(&(table->supervisor[i]), NULL, &personal_supervisor, &(table->philos[i])) != 0)
				exit(1);
			philosophers(&(table->philos[i]));
		}
		table->pids[i] = pid;
	}

	nb = 0;
	pid_t wpid;

	i = 0;
	int status;
	int j = -1;
	while ( i < table->nb_philos )
	{
		wpid = waitpid(-1, &status, 0);
		if ( WIFEXITED(status) && WEXITSTATUS(status) == 42 )
		{
			if ( i == table->nb_philos - 1 )
				break;
		}
		else
		{
			printf("%ldms Philosophers %d just died 💀\n", get_time_ms() - table->start_time, WEXITSTATUS(status));
			while ( ++j < table->nb_philos )
				kill(table->pids[j], SIGKILL);
			break;
		}
		++i;
	}
	return (0);
}
