/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:50:16 by ebonutto          #+#    #+#             */
/*   Updated: 2025/07/03 15:13:31 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/* For printf(), perror() */
# include <stdio.h>

/* For Threads functions (Threads and mutex) */
# include <pthread.h>

/* For malloc(), free(), exit() (only used for the bonus part) */
# include <stdlib.h>

/* For write(), usleep() */
# include <unistd.h>

/* For memset() */
# include <string.h>

/* For gettimeofday() */
# include <sys/time.h>

/* For bool */
# include <stdbool.h>

/* For limits */
# include <limits.h>

/* For time */
# include <time.h>

/* For waitpid() */
# include <sys/wait.h>

/* For SIGKILL */
# include <signal.h>

/* For semaphores */
# include <semaphore.h>

/* For O_CREAT, ... */
# include <fcntl.h>

/* For Debug Only */
# include <stdio.h>

/* Const */
# define ERROR_CODE -42

/* Structures */
typedef struct s_table	t_table;

// typedef struct s_fork
// {
// 	pthread_mutex_t	fork;
// 	int				fork_id;
// }	t_fork;

typedef struct s_philo
{
	pid_t			philo;
	int				philo_id;
	int				meal_counter;
	time_t			last_meal_time;
	sem_t			*meal_time_lock;
	sem_t			*meal_counter_lock;
	t_table			*table;
	bool			done_eating;
}	t_philo;

typedef struct s_table
{
	int			nb_philos;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			nb_miam;
	time_t		start_time;
	sem_t		*forks;
	t_philo		philos[200];
	sem_t		*write_lock;
	sem_t		*miam_lock;
	int			pids[200];
	pthread_t	personal_supervisor[200];
	pthread_t	global_supervisor_death;
	pthread_t	global_supervisor_famine;
	bool		death;
	sem_t		*death_lock;
	
}	t_table;

/* Enum */
typedef enum e_action
{
	EATING,
	SLEEPING,
	THINKING,
	FORKING,
	DYING,
	FULLING
}	t_action;

/* Prototypes */
/* Error */
int		error_message(char *message1, char *message2);

/* Actions */
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
void	safe_print(t_philo *philo, t_action action);

/* Parsing */
int		atoi_check(char *str);

/* Init */
void	get_name(char *name, char *what, int i);
int		init_table(int argc, char **argv, t_table *table);

/* Time */
time_t	get_time_ms(void);
bool	this_is_the_end(t_philo *philo);
bool	is_this_the_end(t_table *table);
void	philo_sleep_check(t_philo *philo, time_t sleep_time);
void	synchronize_all(time_t start);

/* Philo */
int		dinner(t_table *table);

/* Supervisor */
void	*personal_supervisor(void *arg);
void	*global_famine_supervisor(void *arg);

/* Utils */
void	kill_them_all(t_table *table);

#endif
