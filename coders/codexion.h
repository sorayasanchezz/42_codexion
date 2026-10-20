/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sosanche <sosanche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 19:45:02 by sosanche          #+#    #+#             */
/*   Updated: 2026/10/11 19:44:13 by sosanche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <string.h>
# include <limits.h>

typedef struct s_config
{
	int		number_of_coders;
	long	time_to_burnout;
	long	time_to_compile;
	long	time_to_debug;
	long	time_to_refactor;
	int		number_of_compiles_required;
	long	dongle_cooldown;
	char	*scheduler;
}	t_config;

typedef struct s_coder		t_coder;
typedef struct s_simulation	t_simulation;

typedef struct s_heap
{
	t_coder	*items[2];
	int		count;
}	t_heap;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	int				in_use;
	long			available_at;
	t_heap			queue;
}	t_dongle;

struct s_coder
{
	int				id;
	long			last_compile_start;
	int				done;
	int				compilations;
	long			priority;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	t_config		*config;
	t_simulation	*sim;
};

struct s_simulation
{
	t_config		*config;
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_t		*threads;
	pthread_t		monitor;
	int				threads_started;
	int				started;
	long			start_time;
	int				simulation_over;
	int				finished_coders;
	pthread_mutex_t	finished_mutex;
	pthread_mutex_t	scheduler_mutex;
	pthread_cond_t	start_cond;
	pthread_mutex_t	log_mutex;
};

int		parse_args(int argc, char **argv, t_config *config);

int		init_simulation(t_simulation *sim, t_config *config);

int		init_sync(t_simulation *sim);
void	destroy_sync(t_simulation *sim);
void	wait_start(t_simulation *sim);

int		simulation_finished(t_simulation *sim);
void	set_simulation_finished(t_simulation *sim);
void	coder_finished(t_coder *coder);
void	log_action(t_simulation *sim, int id, char *action);
void	announce_burnout(t_simulation *sim, int id);

long	get_time_ms(void);
long	get_timestamp(t_simulation *sim);
void	wait_a_little(t_simulation *sim);
void	sleep_ms(t_simulation *sim, long ms);

void	*coder_routine(void *arg);

void	do_compile(t_coder *coder);
void	do_debug(t_coder *coder);
void	do_refactor(t_coder *coder);

void	destroy_dongles(t_dongle *dongles, int n);
int		take_dongles(t_coder *coder);
void	release_dongles(t_coder *coder);

void	add_request(t_coder *coder);
void	wait_for_turn(t_coder *coder);

t_coder	*heap_peek(t_heap *heap);
void	heap_push(t_heap *heap, t_coder *coder);
void	heap_remove(t_heap *heap, t_coder *coder);
int		goes_first(t_coder *a, t_coder *b);
void	sift_up(t_heap *heap, int i);
void	sift_down(t_heap *heap, int i);

void	*monitor_routine(void *arg);

#endif
