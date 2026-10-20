/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sosanche <sosanche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 14:21:15 by sosanche          #+#    #+#             */
/*   Updated: 2026/10/11 19:43:15 by sosanche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	simulation_finished(t_simulation *sim)
{
	int	finished;

	pthread_mutex_lock(&sim->finished_mutex);
	finished = sim->simulation_over;
	pthread_mutex_unlock(&sim->finished_mutex);
	return (finished);
}

void	set_simulation_finished(t_simulation *sim)
{
	pthread_mutex_lock(&sim->finished_mutex);
	sim->simulation_over = 1;
	pthread_mutex_unlock(&sim->finished_mutex);
}

void	coder_finished(t_coder *coder)
{
	t_simulation	*sim;

	sim = coder->sim;
	coder->done = 1;
	pthread_mutex_lock(&sim->finished_mutex);
	sim->finished_coders++;
	if (sim->finished_coders == sim->config->number_of_coders)
		sim->simulation_over = 1;
	pthread_mutex_unlock(&sim->finished_mutex);
}

void	log_action(t_simulation *sim, int id, char *action)
{
	pthread_mutex_lock(&sim->log_mutex);
	if (!simulation_finished(sim))
		printf("%ld %d %s\n", get_timestamp(sim), id, action);
	pthread_mutex_unlock(&sim->log_mutex);
}

void	announce_burnout(t_simulation *sim, int id)
{
	if (!simulation_finished(sim))
		printf("%ld %d burned out\n", get_timestamp(sim), id);
	set_simulation_finished(sim);
}
