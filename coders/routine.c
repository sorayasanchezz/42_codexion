/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sosanche <sosanche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 20:44:07 by sosanche          #+#    #+#             */
/*   Updated: 2026/10/11 15:58:48 by sosanche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	check_if_finished(t_coder *coder)
{
	if (coder->done)
		return ;
	if (coder->compilations < coder->config->number_of_compiles_required)
		return ;
	coder_finished(coder);
}

static void	wait_alone(t_coder *coder)
{
	log_action(coder->sim, coder->id, "has taken a dongle");
	while (!simulation_finished(coder->sim))
		usleep(1000);
}

static void	stagger_start(t_coder *coder)
{
	int	n;

	n = coder->config->number_of_coders;
	if (coder->id % 2 == 0 || coder->id == n)
		sleep_ms(coder->sim, coder->config->time_to_compile / 2);
}

static void	work_loop(t_coder *coder)
{
	stagger_start(coder);
	while (!simulation_finished(coder->sim))
	{
		do_compile(coder);
		check_if_finished(coder);
		do_debug(coder);
		do_refactor(coder);
	}
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	wait_start(coder->sim);
	if (coder->left_dongle == coder->right_dongle)
		wait_alone(coder);
	else
		work_loop(coder);
	return (NULL);
}
