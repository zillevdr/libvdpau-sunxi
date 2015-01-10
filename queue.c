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
#include <string.h>
#include <stdlib.h>
#include "queue.h"

#undef DEBUG_INFO
#define DEBUG_ERROR

#if defined (DEBUG_ERROR) || defined (DEBUG_INFO)
  #include <stdio.h>
#endif

eQueueError QueueInit(Queue_Struct * queue, size_t num_elems, size_t obj_size)
{
	queue->buffer = malloc(obj_size * num_elems);
	if(queue->buffer == NULL) {
		// no memory available
		#ifdef DEBUG_ERROR
		printf("ERROR: no memory available!\n");
		#endif
		return QUEUE_INIT_ERROR;
	}
	// init Queue-Structure
	queue->buffer_end = (char *)queue->buffer + obj_size * num_elems;
	queue->item_max = num_elems;
	queue->item_count = 0;
	queue->item_size = obj_size;
	queue->input = queue->buffer;
	queue->output = queue->buffer;

	pthread_mutex_init(&queue->mutex, NULL);

	return QUEUE_SUCCESS;
}

void QueueFree(Queue_Struct * queue)
{
	#ifdef DEBUG_INFO
	printf("INFO_FREE: memory freed!\n");
	#endif
	free(queue->buffer);
}

eQueueError QueuePush(Queue_Struct * queue, const void *data)
{
	pthread_mutex_lock(&queue->mutex);
	if(queue->item_count == queue->item_max) {
		// queue is full
		#ifdef DEBUG_ERROR
		printf("ERROR: queue full!\n");
		#endif
		pthread_mutex_unlock(&queue->mutex);
		return QUEUE_FULL;
	}
	// copy data into buffer
	memcpy(queue->input, data, queue->item_size);
	queue->input = (char*)queue->input + queue->item_size;
	if(queue->input == queue->buffer_end) {
		// reached end of buffer, so start at beginning
		queue->input = queue->buffer;
	}
	queue->item_count++;
	#ifdef DEBUG_INFO
	printf("INFO_PUSH: queue items: %i \n",queue->item_count);
	#endif
	pthread_mutex_unlock(&queue->mutex);
	return QUEUE_SUCCESS;
}


eQueueError QueuePop(Queue_Struct * queue, void *data)
{
	pthread_mutex_lock(&queue->mutex);
	if(queue->item_count == 0) {
		// queue is empty
		#ifdef DEBUG_ERROR
		printf("ERROR: queue empty!\n");
		#endif
		pthread_mutex_unlock(&queue->mutex);
		return QUEUE_EMPTY;
	}
	// get data from buffer
	memcpy(data, queue->output, queue->item_size);
	queue->output = (char*)queue->output + queue->item_size;
	if(queue->output == queue->buffer_end) {
		// reached end of buffer, so start at beginning
		queue->output = queue->buffer;
	}
	queue->item_count--;
	#ifdef DEBUG_INFO
	printf("INFO_POP: queue items: %i \n",queue->item_count);
	#endif
	pthread_mutex_unlock(&queue->mutex);
	return QUEUE_SUCCESS;
}

size_t QueueLength(Queue_Struct * queue)
{
	return queue->item_count;
}




