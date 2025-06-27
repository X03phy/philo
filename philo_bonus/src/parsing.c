/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:58:41 by ebonutto          #+#    #+#             */
/*   Updated: 2025/06/27 14:54:26 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	is_only_positive_digit(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
		return (false);
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		if (!('0' <= str[i] && str[i] <= '9'))
			return (false);
		i++;
	}
	return (true);
}

int	atoi_check(char *str)
{
	int		i;
	long	nb;

	if (is_only_positive_digit(str) == false)
		return (-1);
	i = 0;
	if (str[i] == '+')
		i++;
	nb = 0;
	while (str[i])
	{
		nb = nb * 10 + (str[i] - '0');
		if (nb > INT_MAX)
			return (-1);
		i++;
	}
	return ((int)nb);
}

void	get_name(char *name, char *what, int i)
{
	int	n;

	if (i < 10)
		n = 1;
	else if (i < 100)
		n = 2;
	else
		n = 3;
	name[0] = '/';
	name[1] = what[0];
	name[2] = what[1];
	name[3] = what[2];
	name[4] = what[3];
	name[5] = '_';
	name[5 + n + 1] = '\0';
	while (n > 0)
	{
		name[5 + n] = (i % 10) + '0';
		i /= 10;
		--n;
	}
}
