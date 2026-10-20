/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sosanche <sosanche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 12:00:00 by sosanche          #+#    #+#             */
/*   Updated: 2026/09/30 14:01:53 by sosanche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_dongles(t_dongle *dongles, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&dongles[i].mutex);
		i++;
	}
}

static void	lock_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	first = coder->left_dongle;
	second = coder->right_dongle;
	if (coder->id == coder->config->number_of_coders)
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	pthread_mutex_lock(&first->mutex);
	log_action(coder->sim, coder->id, "has taken a dongle");
	pthread_mutex_lock(&second->mutex);
	log_action(coder->sim, coder->id, "has taken a dongle");
}

int	take_dongles(t_coder *coder)
{
	add_request(coder);
	wait_for_turn(coder);
	if (simulation_finished(coder->sim))
		return (0);
	lock_dongles(coder);
	return (1);
}

void	release_dongles(t_coder *coder)
{
	t_simulation	*sim;
	long			free_at;

	sim = coder->sim;
	pthread_mutex_unlock(&coder->left_dongle->mutex);
	pthread_mutex_unlock(&coder->right_dongle->mutex);
	pthread_mutex_lock(&sim->scheduler_mutex);
	free_at = get_timestamp(sim) + coder->config->dongle_cooldown;
	coder->left_dongle->in_use = 0;
	coder->left_dongle->available_at = free_at;
	coder->right_dongle->in_use = 0;
	coder->right_dongle->available_at = free_at;
	pthread_mutex_unlock(&sim->scheduler_mutex);
}
