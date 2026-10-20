/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sosanche <sosanche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 19:59:45 by sosanche          #+#    #+#             */
/*   Updated: 2026/09/30 11:33:24 by sosanche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	give_start_signal(t_simulation *sim)
{
	pthread_mutex_lock(&sim->scheduler_mutex);
	sim->start_time = get_time_ms();
	sim->started = 1;
	pthread_cond_broadcast(&sim->start_cond);
	pthread_mutex_unlock(&sim->scheduler_mutex);
}

static int	start_threads(t_simulation *sim)
{
	int	i;

	if (pthread_create(&sim->monitor, NULL, monitor_routine, sim) != 0)
	{
		fprintf(stderr, "Error: could not create a thread\n");
		return (1);
	}
	i = 0;
	while (i < sim->config->number_of_coders)
	{
		if (pthread_create(&sim->threads[i], NULL, coder_routine,
				&sim->coders[i]) != 0)
		{
			fprintf(stderr, "Error: could not create a thread\n");
			set_simulation_finished(sim);
			break ;
		}
		i++;
	}
	sim->threads_started = i;
	give_start_signal(sim);
	return (0);
}

static void	join_all(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->threads_started)
	{
		pthread_join(sim->threads[i], NULL);
		i++;
	}
	pthread_join(sim->monitor, NULL);
}

static void	destroy_all(t_simulation *sim)
{
	destroy_sync(sim);
	destroy_dongles(sim->dongles, sim->config->number_of_coders);
	free(sim->coders);
	free(sim->dongles);
	free(sim->threads);
}

int	main(int argc, char **argv)
{
	t_config		config;
	t_simulation	sim;

	if (parse_args(argc, argv, &config))
		return (1);
	if (init_simulation(&sim, &config))
	{
		fprintf(stderr, "Error: could not initialize the simulation\n");
		return (1);
	}
	if (start_threads(&sim))
	{
		destroy_all(&sim);
		return (1);
	}
	join_all(&sim);
	destroy_all(&sim);
	return (0);
}
