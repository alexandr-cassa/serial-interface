/*
 * asi_tx.h
 *
 *  Created on: May 17, 2025
 *      Author: Сашенька
 */

#ifndef ASI_ASI_TX_ASI_TX_H_
#define ASI_ASI_TX_ASI_TX_H_

#include "../asi_adapter/asi_adapter.h"

void TX_timerCompleteCallback(void);
void TX_send(uint8_t* data, uint8_t size);

#endif /* ASI_ASI_TX_ASI_TX_H_ */
