/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_sift.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sosanche <sosanche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 12:00:00 by sosanche          #+#    #+#             */
/*   Updated: 2026/10/08 13:10:06 by sosanche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	goes_first(t_coder *a, t_coder *b)
{
	if (a->priority != b->priority)
		return (a->priority < b->priority);
	return (0);
}

static void	swap(t_heap *heap, int a, int b)
{
	t_coder	*tmp;

	tmp = heap->items[a];
	heap->items[a] = heap->items[b];
	heap->items[b] = tmp;
}

void	sift_up(t_heap *heap, int i)
{
	t_coder	**w;
	int		parent;

	w = heap->items;
	parent = (i - 1) / 2;
	while (i > 0 && goes_first(w[i], w[parent]))
	{
		swap(heap, i, parent);
		i = parent;
		parent = (i - 1) / 2;
	}
}

void	sift_down(t_heap *heap, int i)
{
	t_coder	**w;
	int		smallest;
	int		left;
	int		right;

	w = heap->items;
	while (1)
	{
		smallest = i;
		left = 2 * i + 1;
		right = 2 * i + 2;
		if (left < heap->count && goes_first(w[left], w[smallest]))
			smallest = left;
		if (right < heap->count && goes_first(w[right], w[smallest]))
			smallest = right;
		if (smallest == i)
			return ;
		swap(heap, i, smallest);
		i = smallest;
	}
}
