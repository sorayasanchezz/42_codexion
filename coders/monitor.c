/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sosanche <sosanche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 14:36:20 by sosanche          #+#    #+#             */
/*   Updated: 2026/09/30 11:33:22 by sosanche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	coder_burned(t_simulation *sim, t_coder *coder)
{
	return (get_timestamp(sim) - coder->last_compile_start
		>= coder->config->time_to_burnout);
}

static int	find_burned_coder(t_simulation *sim)
{
	int		i;
	int		id;
	t_coder	*coder;

	id = -1;
	i = 0;
	while (i < sim->config->number_of_coders && id == -1)
	{
		coder = &sim->coders[i];
		if (coder_burned(sim, coder))
			id = coder->id;
		i++;
	}
	return (id);
}

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	int				burned_id;

	sim = (t_simulation *)arg;
	wait_start(sim);
	while (!simulation_finished(sim))
	{
		pthread_mutex_lock(&sim->log_mutex);
		burned_id = find_burned_coder(sim);
		if (burned_id != -1)
			announce_burnout(sim, burned_id);
		pthread_mutex_unlock(&sim->log_mutex);
		usleep(1000);
	}
	return (NULL);
}
