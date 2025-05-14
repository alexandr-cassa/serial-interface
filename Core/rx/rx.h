/*
 * tx.h
 *
 *  Created on: Feb 10, 2025
 */

#ifndef RX_RX_H_
#define RX_RX_H_

#include "main.h"

typedef uint8_t (*signal_t)();

typedef struct {
	TIM_HandleTypeDef* htim;
	signal_t source;
	uint32_t pin;
	GPIO_TypeDef* port;
} RX_initStruct;

void RX_init(RX_initStruct initStruct);
void RX_start(uint8_t* data, uint8_t size);

void runSyncFirstStepOnFallingEdgeEvent(void);
void runSyncSecondStateOnRisingEdgeEvent(void);
void runWaitStateOnTimerHalfPeriodEvent(void);
void runReceivingStateOnTimerFullPeriodEvent(void);
void runStopStateOnTimerFullPeriodEvent(void);

void RX_callback(void);
void RX_FallingEdgeCallback(void);
void RX_RisingEdgeCallback(void);


#endif /* RX_RX_H_ */
