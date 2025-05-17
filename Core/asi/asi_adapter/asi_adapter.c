/*
 * asi_adapter.c
 *
 *  Created on: May 17, 2025
 *      Author: Сашенька
 */

#include "asi_adapter.h"

uint8_t readBit(void);
void writeBit(void);

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
