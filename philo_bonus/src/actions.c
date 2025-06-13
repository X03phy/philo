/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:58:19 by ebonutto          #+#    #+#             */
/*   Updated: 2025/06/13 17:04:38 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	safe_print(t_philo *philo, t_action action)
{
	time_t	time;

	pthread_mutex_lock(&(philo->table->write_lock));
	if (is_this_the_end(philo->table) == true)
	{
		pthread_mutex_unlock(&(philo->table->write_lock));
		return ;
	}
	time = get_time_ms() - philo->table->start_time;
	if (action == FORKING)
		printf("%ldms Philosophers %d  has taken a fork 🥄\n",
			time, philo->philo_id);
	else if (action == EATING)
		printf("%ldms Philosophers %d is eating 🍝\n", time, philo->philo_id);
	else if (action == SLEEPING)
		printf("%ldms Philosophers %d is sleeping 😴\n", time, philo->philo_id);
	else if (action == THINKING)
		printf("%ldms Philosophers %d is thinking 🤔\n", time, philo->philo_id);
	else if (action == DYING)
		printf("%ldms Philosophers %d just died 💀\n", time, philo->philo_id);
	else if (action == FULLING)
		printf("%ldms All philosophers finished eating ⭐\n", time);
	pthread_mutex_unlock(&(philo->table->write_lock));
}

void	philo_eat(t_philo *philo)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(&philo->left_fork->fork);
		safe_print(philo, FORKING);
		pthread_mutex_lock(&philo->right_fork->fork);
		safe_print(philo, FORKING);
	}
	else
	{
		pthread_mutex_lock(&philo->right_fork->fork);
		safe_print(philo, FORKING);
		pthread_mutex_lock(&philo->left_fork->fork);
		safe_print(philo, FORKING);
	}
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal_time = get_time_ms();
	pthread_mutex_unlock(&philo->meal_time_lock);
	safe_print(philo, EATING);
	pthread_mutex_lock(&philo->meal_counter_lock);
	philo->meal_counter += 1;
	pthread_mutex_unlock(&philo->meal_counter_lock);
	philo_sleep_check(philo, philo->table->time_to_eat);
	pthread_mutex_unlock(&philo->left_fork->fork);
	pthread_mutex_unlock(&philo->right_fork->fork);
}

void	philo_sleep(t_philo *philo)
{
	safe_print(philo, SLEEPING);
	philo_sleep_check(philo, philo->table->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	time_t	time;

	time = philo->table->time_to_die
		- philo->table->time_to_eat - philo->table->time_to_sleep - 100;
	if (time < 0)
		time = 0;
	else if (time > philo->table->time_to_eat)
		time = philo->table->time_to_eat;
	safe_print(philo, THINKING);
	philo_sleep_check(philo, time);
}
