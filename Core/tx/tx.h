/*
 * rx.h
 *
 *  Created on: Jan 8, 2025
 *      Author: Сашенька
 */

#ifndef TX_TX_H_
#define TX_TX_H_

#include "main.h"

typedef void (*action)(uint8_t);

void TX_init(TIM_HandleTypeDef* htim, action sendAction);
void TX_send(uint8_t* data, uint8_t size);

void TX_timerHalfCompleteCallback();
void TX_timerCompleteCallback();

#endif /* TX_TX_H_ */
