/*
 * asi_rx.c
 *
 *  Created on: May 17, 2025
 *      Author: Сашенька
 */

#include "asi_rx.h"

#define BYTE_SIZE (8u)
#define TIMER(timHandler) ((timHandler)->Instance)
#define counterOf(timer) (TIMER(timer)->CNT)
#define arrOf(timer) (TIMER(timer)->ARR)

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
} rxState_t;

static struct
{
	rxState_t state;
	uint8_t* data;
	uint8_t size;
	uint8_t currentBit;
	uint8_t byte;
	uint8_t currentByte;
	uint32_t arr;
	uint32_t period;
} rx = {0};

static void runPreparing(void);

static void reset(void);
static void readBitIntoByte(void);
static void shiftByteOrPutItInBufferAndGoToStopStateIfCompleted(void);

extern void RX_start(uint8_t* data, uint8_t size)
{
	rx.data = data;
	rx.size = size;

	runPreparing();
}

extern void RX_runSyncFirstStepOnFallingEdgeEvent(void) {
	if(ready == rx.state) {
		setReceiverTimerCntValue(0);
		setReceiverTimerArrValue(-1);
		receiverTimerBaseStart();
		rx.state = sync;
	}
}

extern void RX_runSyncSecondStateOnRisingEdgeEvent(void) {
	if (sync == rx.state) {
		rx.period = getReceiverTimerCntValue();

		setReceiverDataPinStateToGpioInput();

		/* Reconfigure timer */
		receiverTimerBaseStop();
		setReceiverTimerCntValue(0);
		setReceiverTimerArrValue(rx.period / 2);
		receiverTimerBaseStartIT();

		rx.state = wait;
	}
}

extern void RX_runWaitStateOnTimerHalfPeriodEvent(void) {
	if (wait == rx.state) {
		setReceiverTimerCntValue(0);
		setReceiverTimerArrValue(rx.period);
		rx.state = receiving;
	}
}

extern void RX_runReceivingStateOnTimerFullPeriodEvent(void) {
	if (receiving == rx.state) {
		readBitIntoByte();
		shiftByteOrPutItInBufferAndGoToStopStateIfCompleted();
	}
}

extern void RX_runStopStateOnTimerFullPeriodEvent(void) {
	if (stop == rx.state) {
		receiverTimerBaseStopIT();

		/*
			May be finish is redundant?
		 */
		if (rx.currentByte >= rx.size) {
			reset();
			rx.state = idle;
		} else {
			runPreparing();
		}
	}
}


static void runPreparing(void) {
	setReceiverDataPinStateToEXTI();
	rx.state = ready;
}

static void reset(void)
{
	rx.currentBit = 0;
	rx.state = idle;
	rx.byte = 0;
	rx.currentByte = 0;
	rx.size = 0;
}

static void readBitIntoByte(void)
{
	uint8_t bit = (!readBit()) & (1u);
	rx.byte |= bit;
}

static void shiftByteOrPutItInBufferAndGoToStopStateIfCompleted(void)
{
	if ((BYTE_SIZE - 1u) <= rx.currentBit)
	{
		rx.data[rx.size - 1 - rx.currentByte++] = rx.byte;
		rx.byte = 0;
		rx.currentBit = 0;

		rx.state = stop;
	}
	else
	{
		rx.byte <<= 1;
		rx.currentBit++;
	}
}
