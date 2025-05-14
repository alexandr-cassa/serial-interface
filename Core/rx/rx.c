/*
 * rx.c
 *
 *  Created on: Feb 10, 2025
 */

#include "rx.h"

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
} rxState;

static struct
{
	signal_t  signalState;
	rxState state;
	uint8_t* data;
	uint8_t size;
	uint8_t currentBit;
	uint8_t byte;
	uint8_t currentByte;
	uint32_t arr;
	uint32_t period;
	TIM_HandleTypeDef* timer;
} rx = {0};

static void switchDataPinMode(uint32_t mode);

static void runPreparing(void);

static void reset(void);
static void readBitIntoByte(void);
static void shiftByteOrPutItInBufferAndGoToStopStateIfCompleted(void);

extern void RX_init(RX_initStruct initStruct)
{
	rx.timer = initStruct.htim;
	rx.signalState = initStruct.source;
}


extern void RX_start(uint8_t* data, uint8_t size)
{
	rx.data = data;
	rx.size = size;

	runPreparing();
}

extern void runSyncFirstStepOnFallingEdgeEvent(void) {
	if(ready == rx.state) {
		counterOf(rx.timer) = 0;
		arrOf(rx.timer) = -1;
		HAL_TIM_Base_Start(rx.timer);
		rx.state = sync;
	}
}

extern void runSyncSecondStateOnRisingEdgeEvent(void) {
	if (sync == rx.state) {
		rx.period = counterOf(rx.timer);

		switchDataPinMode(GPIO_MODE_INPUT);

		/* Reconfigure timer */
		HAL_TIM_Base_Stop(rx.timer);
		counterOf(rx.timer) = 0;
		arrOf(rx.timer) = rx.period / 2;
		HAL_TIM_Base_Start_IT(rx.timer);

		rx.state = wait;
	}
}

extern void runWaitStateOnTimerHalfPeriodEvent(void) {
	if (wait == rx.state) {
		counterOf(rx.timer) = 0;
		arrOf(rx.timer) = rx.period;

		rx.state = receiving;
	}
}

extern void runReceivingStateOnTimerFullPeriodEvent(void) {
	if (receiving == rx.state) {
		readBitIntoByte();
		shiftByteOrPutItInBufferAndGoToStopStateIfCompleted();
	}
}

extern void runStopStateOnTimerFullPeriodEvent(void) {
	if (stop == rx.state) {
		HAL_TIM_Base_Stop_IT(rx.timer);

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

static void runPreparing(void) {
	switchDataPinMode(GPIO_MODE_IT_RISING_FALLING);
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
	uint8_t bit =  (!rx.signalState()) & (1u);
	rx.byte |= bit;
	HAL_GPIO_TogglePin(DEBUG_PIN_GPIO_Port, DEBUG_PIN_Pin);
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
