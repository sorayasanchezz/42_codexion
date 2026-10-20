/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sosanche <sosanche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 21:00:00 by sosanche          #+#    #+#             */
/*   Updated: 2026/09/30 11:33:28 by sosanche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	heap_find(t_heap *heap, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < heap->count)
	{
		if (heap->items[i] == coder)
			return (i);
		i++;
	}
	return (-1);
}

t_coder	*heap_peek(t_heap *heap)
{
	if (heap->count == 0)
		return (NULL);
	return (heap->items[0]);
}

void	heap_push(t_heap *heap, t_coder *coder)
{
	heap->items[heap->count] = coder;
	heap->count++;
	sift_up(heap, heap->count - 1);
}

void	heap_remove(t_heap *heap, t_coder *coder)
{
	int	i;

	i = heap_find(heap, coder);
	if (i == -1)
		return ;
	heap->count--;
	heap->items[i] = heap->items[heap->count];
	if (i < heap->count)
	{
		sift_up(heap, i);
		sift_down(heap, i);
	}
}
