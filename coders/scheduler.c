/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sosanche <sosanche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 21:00:00 by sosanche          #+#    #+#             */
/*   Updated: 2026/09/30 14:03:10 by sosanche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	can_take(t_dongle *dongle, t_coder *coder, long now)
{
	t_coder	*first;

	if (dongle->in_use || now < dongle->available_at)
		return (0);
	first = heap_peek(&dongle->queue);
	if (first != coder && goes_first(first, coder))
		return (0);
	return (1);
}

static int	my_turn(t_coder *coder, long now)
{
	if (!can_take(coder->left_dongle, coder, now))
		return (0);
	if (!can_take(coder->right_dongle, coder, now))
		return (0);
	return (1);
}

void	add_request(t_coder *coder)
{
	t_simulation	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&sim->scheduler_mutex);
	if (strcmp(coder->config->scheduler, "fifo") == 0)
		coder->priority = get_timestamp(sim);
	else
		coder->priority = coder->last_compile_start
			+ coder->config->time_to_burnout;
	heap_push(&coder->left_dongle->queue, coder);
	heap_push(&coder->right_dongle->queue, coder);
	pthread_mutex_unlock(&sim->scheduler_mutex);
}

void	wait_for_turn(t_coder *coder)
{
	t_simulation	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&sim->scheduler_mutex);
	while (!my_turn(coder, get_timestamp(sim)) && !simulation_finished(sim))
		wait_a_little(sim);
	heap_remove(&coder->left_dongle->queue, coder);
	heap_remove(&coder->right_dongle->queue, coder);
	if (!simulation_finished(sim))
	{
		coder->left_dongle->in_use = 1;
		coder->right_dongle->in_use = 1;
	}
	pthread_mutex_unlock(&sim->scheduler_mutex);
}
