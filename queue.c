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

#include "queue.h"
#include <inttypes.h>
#include <string.h>

eQueueError QueuePush(Queue_Struct * queue, Task_Struct data)
{
	uint8_t next = ((queue->InIndex + 1) & QUEUE_MASK);
	if (queue->OutIndex == next) {
		queue->count = QUEUE_LEN;
		return QUEUE_FULL;
	}
	queue->data[queue->InIndex] = data;
	queue->InIndex = next;
	queue->count++;
	return QUEUE_SUCCESS;
}

eQueueError QueuePop(Queue_Struct * queue, Task_Struct * ptr_data)
{
	if (queue->OutIndex == queue->InIndex) {
		queue->count = 0;
		return QUEUE_EMPTY;
	}
	*ptr_data = queue->data[queue->OutIndex];
	queue->OutIndex = (queue->OutIndex+1) & QUEUE_MASK;
	queue->count--;
	return QUEUE_SUCCESS;
}

uint32_t QueueLength(Queue_Struct * queue)
{
	return queue->count;
}

void QueueInit(Queue_Struct * queue)
{
	memset(queue, 0, sizeof(Queue_Struct));
}


