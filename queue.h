/*
 * Copyright (c) 2014 Manuel Feser <manuelfeser@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <inttypes.h>
#include <vdpau/vdpau.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct {
	uint32_t				clip_width;
	uint32_t				clip_height;
	VdpOutputSurface		surface;
	VdpPresentationQueue	queue_id;
}Task_Struct;


/* enum with error codes */
typedef enum {
	QUEUE_SUCCESS,
	QUEUE_FULL,
	QUEUE_EMPTY,
	QUEUE_INIT_ERROR
}eQueueError;


typedef struct
{
	void *buffer;			// data buffer
	void *buffer_end;		// end of buffer
	size_t item_count;		// number of items in the buffer
	size_t item_max;		// max number of items in the buffer
	size_t item_size;		// size of single item
	void *input ; 			// input position
	void *output;			// output position
	pthread_mutex_t mutex;  // mutex to make queue thread-safe
}Queue_Struct;


eQueueError QueueInit(Queue_Struct * queue, size_t num_elems, size_t obj_size);
void QueueFree(Queue_Struct * queue);
eQueueError QueuePush(Queue_Struct * queue, const void *data);
eQueueError QueuePop(Queue_Struct * queue, void *data);
size_t QueueLength(Queue_Struct * queue);
