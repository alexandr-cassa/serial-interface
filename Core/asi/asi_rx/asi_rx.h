/*
 * asi_rx.h
 *
 *  Created on: May 17, 2025
 *      Author: Сашенька
 */

#ifndef ASI_ASI_RX_ASI_RX_H_
#define ASI_ASI_RX_ASI_RX_H_

#include "../asi_adapter/asi_adapter.h"

void RX_read(uint8_t* data, uint16_t size);

void RX_runSyncFirstStepOnFallingEdgeEvent(void);
void RX_runSyncSecondStateOnRisingEdgeEvent(void);
void RX_runWaitStateOnTimerHalfPeriodEvent(void);
void RX_runReceivingStateOnTimerFullPeriodEvent(void);
void RX_runStopStateOnTimerFullPeriodEvent(void);

#endif /* ASI_ASI_RX_ASI_RX_H_ */
