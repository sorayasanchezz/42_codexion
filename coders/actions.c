/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sosanche <sosanche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 14:00:00 by sosanche          #+#    #+#             */
/*   Updated: 2026/10/11 19:09:37 by sosanche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	start_compiling(t_coder *coder)
{
	t_simulation	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&sim->log_mutex);
	coder->last_compile_start = get_timestamp(sim);
	if (!simulation_finished(sim))
		printf("%ld %d is compiling\n", coder->last_compile_start, coder->id);
	pthread_mutex_unlock(&sim->log_mutex);
}

void	do_compile(t_coder *coder)
{
	if (!take_dongles(coder))
		return ;
	start_compiling(coder);
	sleep_ms(coder->sim, coder->config->time_to_compile);
	coder->compilations++;
	log_action(coder->sim, coder->id, "is debugging");
	release_dongles(coder);
}

static void	do_timed_action(t_coder *coder, char *action, long ms)
{
	if (simulation_finished(coder->sim))
		return ;
	log_action(coder->sim, coder->id, action);
	sleep_ms(coder->sim, ms);
}

void	do_debug(t_coder *coder)
{
	sleep_ms(coder->sim, coder->config->time_to_debug);
}

void	do_refactor(t_coder *coder)
{
	do_timed_action(coder, "is refactoring", coder->config->time_to_refactor);
}
