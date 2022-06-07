

#include "circularBuffer.h"


uint16_t cb_init(cb_t *cb)
{
	cb->size = 1 << 5;
	cb->buffer = malloc(sizeof(s_data_t)*cb->size);	// get block of memory
	cb->read = 0;
	cb->write = 0;

	return cb->size;	// return size of buffer
}

/**
* @return used space; 0 for empty, (size - 1) for full
* MES page 179
*/
uint16_t cb_used(cb_t *cb)
{
	return ((cb->write - cb->read) & (cb->size - 1));
}

cb_status_t cb_write(cb_t *cb, s_data_t data)
{
	if(cb_used(cb) == (cb->size - 1))
	{
		return cb_status_BUFFER_FULL;
	}
	cb->buffer[cb->write] = data;
	cb->write = (cb->write + 1) & (cb->size - 1); // must be atomic
	return cb_status_t_OKAY;
}

cb_status_t cb_read(cb_t *cb, s_data_t *data)
{
	if(cb_used(cb) == 0)
	{
		return cb_status_BUFFER_EMPTY;
	}
	*data = cb->buffer[cb->read];
	cb->read = (cb->read + 1) & (cb->size -1);
	return cb_status_t_OKAY;
}