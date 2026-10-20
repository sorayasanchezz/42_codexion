/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sosanche <sosanche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 17:48:38 by sosanche          #+#    #+#             */
/*   Updated: 2026/09/30 11:33:09 by sosanche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long	get_timestamp(t_simulation *sim)
{
	return (get_time_ms() - sim->start_time);
}

void	wait_a_little(t_simulation *sim)
{
	pthread_mutex_unlock(&sim->scheduler_mutex);
	usleep(200);
	pthread_mutex_lock(&sim->scheduler_mutex);
}

void	sleep_ms(t_simulation *sim, long ms)
{
	long	end;

	end = get_time_ms() + ms;
	while (get_time_ms() < end && !simulation_finished(sim))
		usleep(100);
}
