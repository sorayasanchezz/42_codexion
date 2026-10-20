/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sosanche <sosanche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 19:37:19 by sosanche          #+#    #+#             */
/*   Updated: 2026/09/30 11:33:18 by sosanche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long	parse_number(char *str)
{
	long	num;
	int		i;

	if (str[0] == '\0')
		return (-1);
	num = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		num = num * 10 + (str[i] - '0');
		if (num > INT_MAX)
			return (-1);
		i++;
	}
	return (num);
}

static int	invalid_config(t_config *config)
{
	if (config->number_of_coders < 1)
		return (1);
	if (config->number_of_compiles_required < 1)
		return (1);
	if (config->time_to_burnout < 0)
		return (1);
	if (config->time_to_compile < 0)
		return (1);
	if (config->time_to_debug < 0)
		return (1);
	if (config->time_to_refactor < 0)
		return (1);
	if (config->dongle_cooldown < 0)
		return (1);
	if (strcmp(config->scheduler, "fifo") != 0
		&& strcmp(config->scheduler, "edf") != 0)
		return (1);
	return (0);
}

static void	print_usage(void)
{
	fprintf(stderr, "Error: invalid arguments\n");
	fprintf(stderr, "Usage: ./codexion number_of_coders time_to_burnout ");
	fprintf(stderr, "time_to_compile time_to_debug time_to_refactor ");
	fprintf(stderr, "number_of_compiles_required dongle_cooldown scheduler\n");
	fprintf(stderr, "Numbers: integers >= 0 (coders and compiles >= 1). ");
	fprintf(stderr, "Scheduler: fifo or edf\n");
}

int	parse_args(int argc, char **argv, t_config *config)
{
	if (argc != 9)
	{
		print_usage();
		return (1);
	}
	config->number_of_coders = parse_number(argv[1]);
	config->time_to_burnout = parse_number(argv[2]);
	config->time_to_compile = parse_number(argv[3]);
	config->time_to_debug = parse_number(argv[4]);
	config->time_to_refactor = parse_number(argv[5]);
	config->number_of_compiles_required = parse_number(argv[6]);
	config->dongle_cooldown = parse_number(argv[7]);
	config->scheduler = argv[8];
	if (invalid_config(config))
	{
		print_usage();
		return (1);
	}
	return (0);
}
