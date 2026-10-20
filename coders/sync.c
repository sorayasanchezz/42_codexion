/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sosanche <sosanche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 13:30:00 by sosanche          #+#    #+#             */
/*   Updated: 2026/09/30 11:33:12 by sosanche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_sync(t_simulation *sim)
{
	if (pthread_mutex_init(&sim->finished_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&sim->scheduler_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->finished_mutex);
		return (1);
	}
	if (pthread_cond_init(&sim->start_cond, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->finished_mutex);
		pthread_mutex_destroy(&sim->scheduler_mutex);
		return (1);
	}
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->finished_mutex);
		pthread_mutex_destroy(&sim->scheduler_mutex);
		pthread_cond_destroy(&sim->start_cond);
		return (1);
	}
	return (0);
}

void	destroy_sync(t_simulation *sim)
{
	pthread_mutex_destroy(&sim->finished_mutex);
	pthread_mutex_destroy(&sim->scheduler_mutex);
	pthread_cond_destroy(&sim->start_cond);
	pthread_mutex_destroy(&sim->log_mutex);
}

void	wait_start(t_simulation *sim)
{
	pthread_mutex_lock(&sim->scheduler_mutex);
	while (!sim->started)
		pthread_cond_wait(&sim->start_cond, &sim->scheduler_mutex);
	pthread_mutex_unlock(&sim->scheduler_mutex);
}
