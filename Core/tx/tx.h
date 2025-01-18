/*
 * rx.h
 *
 *  Created on: Jan 8, 2025
 *      Author: Сашенька
 */

#ifndef TX_TX_H_
#define TX_TX_H_

#include "main.h"

typedef void (*action)();

void TX_init(action action);
void TX_send(uint8_t* data, uint8_t size);
void TX_timerCallback(void);

#endif /* TX_TX_H_ */
