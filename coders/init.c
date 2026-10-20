/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sosanche <sosanche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 00:37:33 by sosanche          #+#    #+#             */
/*   Updated: 2026/10/07 14:11:35 by sosanche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	init_dongle_fields(t_dongle *dongle)
{
	dongle->in_use = 0;
	dongle->available_at = 0;
	dongle->queue.count = 0;
}

static t_dongle	*create_dongles(t_config *config)
{
	t_dongle	*dongles;
	int			i;

	dongles = malloc(sizeof(t_dongle) * config->number_of_coders);
	if (!dongles)
		return (NULL);
	i = 0;
	while (i < config->number_of_coders)
	{
		init_dongle_fields(&dongles[i]);
		if (pthread_mutex_init(&dongles[i].mutex, NULL) != 0)
		{
			while (i > 0)
			{
				i--;
				pthread_mutex_destroy(&dongles[i].mutex);
			}
			free(dongles);
			return (NULL);
		}
		i++;
	}
	return (dongles);
}

static void	init_coders(t_simulation *sim, t_config *config)
{
	int	n;
	int	i;

	n = config->number_of_coders;
	i = 0;
	while (i < n)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compilations = 0;
		sim->coders[i].last_compile_start = 0;
		sim->coders[i].done = 0;
		sim->coders[i].priority = 0;
		sim->coders[i].left_dongle = &sim->dongles[i];
		sim->coders[i].right_dongle = &sim->dongles[(i + 1) % n];
		sim->coders[i].config = config;
		sim->coders[i].sim = sim;
		i++;
	}
}

static int	alloc_resources(t_simulation *sim, t_config *config)
{
	int	n;

	n = config->number_of_coders;
	sim->dongles = create_dongles(config);
	sim->coders = malloc(sizeof(t_coder) * n);
	sim->threads = malloc(sizeof(pthread_t) * n);
	if (sim->dongles && sim->coders && sim->threads)
	{
		init_coders(sim, config);
		return (0);
	}
	if (sim->dongles)
		destroy_dongles(sim->dongles, n);
	free(sim->dongles);
	free(sim->coders);
	free(sim->threads);
	return (1);
}

int	init_simulation(t_simulation *sim, t_config *config)
{
	sim->start_time = 0;
	sim->threads_started = 0;
	sim->started = 0;
	sim->simulation_over = 0;
	sim->finished_coders = 0;
	sim->config = config;
	if (init_sync(sim))
		return (1);
	if (alloc_resources(sim, config))
	{
		destroy_sync(sim);
		return (1);
	}
	return (0);
}
