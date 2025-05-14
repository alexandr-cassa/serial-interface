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
	start,
	sync,
	wait,
	send,
	stop,
	finish,
	states
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
	uint8_t debug;
	TIM_HandleTypeDef* timerHandler;
} tx;

static void sendByte(void);

static void runIdle(void);
static void runStart(void);
static void runSync(void);
static void runWait(void);
static void runSend(void);
static void runStop(void);
static void runFinish(void);

static void reset(void);
static void sendBit(uint8_t byte, uint8_t bitLocation);

static callable run[states] = {
	runIdle,
	runStart,
	runSync,
	runWait,
	runSend,
	runStop,
	runFinish
};

void TX_init(TIM_HandleTypeDef* htim, action sendAction)
{
	tx.sendAction  = sendAction;
	tx.timerHandler = htim;
	reset();
}

void TX_send(uint8_t* data, uint8_t size)
{
	tx.dataToTransmit = data;
	tx.numberOfBytes = size;
	tx.state = start;
}

static void runIdle(void) {

}

static void runStart(void) {
	tx.state = sync;
}

static void runSync(void) {
	tx.sendAction(0);
	tx.state = wait;
}

static void runWait(void) {
	tx.sendAction(1);
	tx.state = send;
}

static void runSend(void) {
	if(tx.currentByte < tx.numberOfBytes)
	{
		tx.debug = tx.dataToTransmit[tx.numberOfBytes - tx.currentByte - 1];
		sendBit
		(
			tx.dataToTransmit[tx.numberOfBytes - tx.currentByte - 1],
			tx.currentBit++
		);
		if(tx.currentBit >= BYTE_SIZE)
		{
			tx.currentByte++;
			tx.currentBit = 0;
			tx.state = stop;
		}
	}
}

static void runStop(void) {
	tx.sendAction(1);
	tx.state = (tx.numberOfBytes <= tx.currentByte)
			? finish
			: sync;
}

static void runFinish(void) {
	reset();
}

static inline void sendByte(void)
{
	if(tx.currentByte < tx.numberOfBytes)
	{
		tx.debug = tx.dataToTransmit[tx.numberOfBytes - tx.currentByte - 1];
		sendBit
		(
			tx.dataToTransmit[tx.numberOfBytes - tx.currentByte - 1],
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

static void sendBit(uint8_t byte, uint8_t bitLocation)
{
	uint8_t bit = (byte >> (7 - bitLocation)) & FIRST_BIT_MASK;
	tx.sendAction(bit);
}

static void reset(void)
{
	tx.state = idle;
	tx.dataToTransmit = NULL;
	tx.currentByte = 0;
	tx.numberOfBytes = 0;
	tx.currentBit = 0;
	tx.sendAction(1);
}

void TX_timerHalfCompleteCallback()
{
	if (idle == tx.state) return;

	if(start == tx.state)
	{
		tx.clock = 1;
		tx.state = send;
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
}

void TX_timerCompleteCallback() {
	if (states > tx.state) {
		run[tx.state]();
	} else {
		tx.state = idle;
	}
}
