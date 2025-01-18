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

static struct
{
	txState  state;
	uint8_t* dataToTransmit;
	uint8_t  numberOfBytes;
	uint8_t  currentByte;
	uint8_t  currentBit;
	action   action;
} tx;

static inline void idleState(void);
static inline void startState(void);
static inline void sendState(void);
static inline void stopState(void);

static inline void reset(void);
static inline void sendBit(uint8_t byte, uint8_t bitLocation);

void TX_init(action action)
{
	tx.action = action;
	reset();
}

void TX_send(uint8_t* data, uint8_t size)
{
	tx.dataToTransmit = data;
	tx.numberOfBytes = size;
	tx.state = start;
}

void TX_timerCallback(void)
{
	switch(tx.state)
	{
	case idle:
		idleState();
		break;
	case start:
		startState();
		break;
	case sending:
		sendState();
		break;
	case stop:
		stopState();
		break;
	default:
		break;
	}
}

static inline void idleState(void)
{
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, SET);
}

static inline void startState(void)
{
	tx.state = sending;
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, RESET);
}

static inline void sendState(void)
{
	if(tx.currentByte < tx.numberOfBytes)
	{
		if(tx.currentBit >= BYTE_SIZE)
		{
			tx.currentByte++;
			tx.currentBit = 0;
		}
		else
		{
			sendBit
			(
				tx.dataToTransmit[tx.currentByte],
				tx.currentBit
			);
			tx.currentBit++;
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
	tx.action(bit);
}

static inline void reset(void)
{
	tx.state = idle;
	tx.dataToTransmit = NULL;
	tx.currentByte = 0;
	tx.numberOfBytes = 0;
}
