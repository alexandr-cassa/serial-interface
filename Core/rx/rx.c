/*
 * rx.c
 *
 *  Created on: Feb 10, 2025
 */

#include "rx.h"

#define BYTE_SIZE (8u)
#define TIMER(timHandler) ((timHandler)->Instance)

TIM_TypeDef* timer;

typedef enum
{
	idle,
	preparing,
	ready,
	sync,
	wait,
	receiving,
	stop,
	finish,
	states
} rxState;

static struct
{
	signal  signalState;
	rxState state;
	uint8_t* data;
	uint8_t size;
	uint8_t currentBit;
	uint8_t byte;
	uint8_t currentByte;
	uint32_t arr;
} rx;

static void switchDataPinMode(uint32_t mode);

static void runIdle(void);
static void runPreparing(void);
static void runReady(void);
static void runSync(void);
static void runWait(void);
static void runRecieving(void);
static void runStop(void);
static void runFinishing(void);

static inline void reset(void);
static inline void readBitIntoByte(void);
static inline void shiftByteOrPutItInBufferIfCompleted(void);
static inline void resetRxIfBufferIsFull(void);

static void switchDataPinMode(uint32_t mode) {
	HAL_GPIO_DeInit(RX_DATA_GPIO_Port, RX_DATA_Pin);

	GPIO_InitTypeDef initStruct = {
		.Pin = RX_DATA_Pin,
		.Mode = mode,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_LOW
	};
	HAL_GPIO_Init(RX_DATA_GPIO_Port, &initStruct);
}

void RX_init(RX_initStruct initStruct)
{
	rx.signalState = source;
}


void RX_start(uint8_t* data, uint8_t size)
{
	rx.data = data;
	rx.size = size;
	rx.state = ready;
}

void RX_callback(void)
{
	if(idle != rx.state)
	{
		readBitIntoByte();
		shiftByteOrPutItInBufferIfCompleted();
		resetRxIfBufferIsFull();
	}
}

static void runIdle(void) {
	/* Nothing to do */
}

static void runPreparing(void) {
	switchDataPinMode(GPIO_MODE_IT_RISING_FALLING);
	rx.state = ready;
}

static void runReady(void) {

}

static void runSync(void) {

}

static void runWait(void) {

}

static void runRecieving(void) {

}

static void runStop(void) {

}

static void runFinishing(void) {

}

static inline void reset(void)
{
	rx.currentBit = 0;
	rx.state = idle;
	rx.byte = 0;
	rx.currentByte = 0;
	rx.size = 0;
}

static inline void readBitIntoByte(void)
{
	rx.byte |= (!rx.signalState()) & (1u);
}

static inline void shiftByteOrPutItInBufferIfCompleted(void)
{
	if ((BYTE_SIZE - 1u) <= rx.currentBit)
	{
		rx.data[rx.size - 1 - rx.currentByte++] = rx.byte;
		rx.byte = 0;
		rx.currentBit = 0;
	}
	else
	{
		rx.byte <<= 1;
		rx.currentBit++;
	}
}

static inline void resetRxIfBufferIsFull(void)
{
	if (rx.currentByte >= rx.size)
	{
		reset();
	}
}

void RX_FallingEdgeCallback(void) {
	timer->CNT = 0;
}

void RX_RisingEdgeCallback(void) {
	rx.arr = timer->CNT;
	timer->CNT = 0;
}
