/*
 * asi_adapter.h
 *
 *  Created on: May 17, 2025
 *      Author: Сашенька
 */

#ifndef ASI_ASI_ADAPTER_ASI_ADAPTER_H_
#define ASI_ASI_ADAPTER_ASI_ADAPTER_H_

#include "stdint.h"
/* User includes */
#include "main.h"
/* End of user includes */

uint8_t readBit(void);
void writeBit(uint8_t bit);

void setReceiverDataPinStateToGpioInput(void);
void setReceiverDataPinStateToEXTI(void);

uint32_t getReceiverTimerCntValue(void);
void     setReceiverTimerCntValue(uint32_t val);

uint32_t getReceiverTimerArrValue(void);
void     setReceiverTimerArrValue(uint32_t val);

void receiverTimerBaseStart(void);
void receiverTimerBaseStop(void);
void receiverTimerBaseStartIT(void);
void receiverTimerBaseStopIT(void);





#endif /* ASI_ASI_ADAPTER_ASI_ADAPTER_H_ */
