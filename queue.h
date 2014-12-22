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


#define         QUEUE_LEN	16 				// has to be 2^n
#define         QUEUE_MASK	(QUEUE_LEN - 1)


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
	QUEUE_EMPTY
}eQueueError;


typedef struct
{
	Task_Struct data[QUEUE_LEN];// data
	uint8_t InIndex; 			// next input position
	uint8_t OutIndex; 			// read position
	uint8_t count;				// number of entries
}Queue_Struct;


void QueueInit(Queue_Struct * queue);
eQueueError QueuePush(Queue_Struct * queue, Task_Struct data);
eQueueError QueuePop(Queue_Struct * queue, Task_Struct * ptr_data);
uint32_t QueueLength(Queue_Struct * queue);
