/*
 * tx.h
 *
 *  Created on: Feb 10, 2025
 */

#ifndef RX_RX_H_
#define RX_RX_H_

#include "main.h"

typedef uint8_t (*signal)();

typedef struct {
	TIM_HandleTypeDef* htim;
	GPIO_TypeDef* port;
	uint32_t pin;
	signal source;
} RX_initStruct;

void RX_init(RX_initStruct initStruct);
void RX_start(uint8_t* data, uint8_t size);

void RX_callback(void);
void RX_FallingEdgeCallback(void);
void RX_RisingEdgeCallback(void);


#endif /* RX_RX_H_ */
