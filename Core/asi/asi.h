/*
 * asi.h
 *
 *  Created on: May 17, 2025
 *      Author: Сашенька
 */

#ifndef ASI_ASI_H_
#define ASI_ASI_H_

#include "asi_adapter/asi_adapter.h"

void ASI_init();

void ASI_write(uint8_t* data, uint16_t size);
void ASI_read(uint8_t* data, uint16_t size);

void ASI_transmitterTimerCallback(void);
void ASI_receiverTimerCallback(void);
void ASI_receiverFallingEdgeDataPinCallback(void);
void ASI_receiverRisingEdgeDataPinCallback(void);

#endif /* ASI_ASI_H_ */
