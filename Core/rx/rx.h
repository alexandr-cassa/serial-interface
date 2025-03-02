/*
 * tx.h
 *
 *  Created on: Feb 10, 2025
 *      Author: Сашенька
 */

#ifndef RX_RX_H_
#define RX_RX_H_

#include "main.h"

typedef uint8_t (*signal)();

void RX_init(signal source);
void RX_start(uint8_t* data, uint8_t size);
void RX_callback(void);

#endif /* RX_RX_H_ */
