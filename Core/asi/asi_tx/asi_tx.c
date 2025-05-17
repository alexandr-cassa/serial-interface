/*
 * asi_tx.c
 *
 *  Created on: May 17, 2025
 *      Author: Сашенька
 */

#include "asi_tx.h"

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
} txState_t;

typedef void (*callable)();

static struct
{
	txState_t  state;
	uint8_t* dataToTransmit;
	uint8_t  numberOfBytes;
	uint8_t  currentByte;
	uint8_t  currentBit;
	uint8_t  clock;
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

extern void TX_send(uint8_t* data, uint8_t size) {
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
	writeBit(0);
	tx.state = wait;
}

static void runWait(void) {
	writeBit(1);
	tx.state = send;
}

static void runSend(void) {
	if(tx.currentByte < tx.numberOfBytes)
	{
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
	writeBit(1);
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
	writeBit(bit);
}

static void reset(void)
{
	tx.state = idle;
	tx.dataToTransmit = NULL;
	tx.currentByte = 0;
	tx.numberOfBytes = 0;
	tx.currentBit = 0;
	writeBit(1);
}

extern void TX_timerCompleteCallback(void) {
	if (states > tx.state) {
		run[tx.state]();
	} else {
		tx.state = idle;
	}
}

