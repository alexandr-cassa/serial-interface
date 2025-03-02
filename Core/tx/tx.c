/*
 * rx.c
 *
 *  Created on: Jan 8, 2025
 *      Author: Сашенька
 */

#include "tx.h"

#define FIRST_BIT_MASK (0x1U)
#define BYTE_SIZE (8u)

typedef enum
{
	idle,
	sync,
	start,
	sending,
	stop
} txState;

typedef void (*callable)();

static struct
{
	txState  state;
	uint8_t* dataToTransmit;
	uint8_t  numberOfBytes;
	uint8_t  currentByte;
	uint8_t  currentBit;
	uint8_t  clock;
	action   sendAction;
	action   clockAction;
} tx;

static void sendByte(void);

static inline void reset(void);
static inline void sendBit(uint8_t byte, uint8_t bitLocation);

void TX_init(action sendAction, action clockAction)
{
	tx.sendAction  = sendAction;
	tx.clockAction = clockAction;
	reset();
}

void TX_send(uint8_t* data, uint8_t size)
{
	tx.dataToTransmit = data;
	tx.numberOfBytes = size;
	tx.state = start;
}

static inline void sendByte(void)
{
	if(tx.currentByte < tx.numberOfBytes)
	{
		sendBit
		(
			tx.dataToTransmit[tx.currentByte],
			tx.currentBit++
		);
		if(tx.currentBit >= BYTE_SIZE)
		{
			tx.currentByte++;
			tx.currentBit = 0;
		}
	}
	else
	{
		tx.state = stop;
	}
}

static inline void stopState(void)
{
	reset();
}

static inline void sendBit(uint8_t byte, uint8_t bitLocation)
{
	uint8_t bit = (byte >> bitLocation) & FIRST_BIT_MASK;
	tx.sendAction(bit);
}

static inline void reset(void)
{
	tx.state = idle;
	tx.dataToTransmit = NULL;
	tx.currentByte = 0;
	tx.numberOfBytes = 0;
}

void TX_timerHalfCompleteCallback()
{
	if (idle == tx.state) return;

	if(start == tx.state)
	{
		tx.clock = 1;
		tx.state = sending;
	}
	else if(stop == tx.state)
	{
		tx.clock = 0;
		reset();
	}
	else
	{
		tx.clock = !tx.clock;
	}
	tx.clockAction(tx.clock);
}

void TX_timerCompleteCallback()
{
	if (idle == tx.state) return;

	if (1 == tx.clock)
	{
		sendByte();
	}
}
