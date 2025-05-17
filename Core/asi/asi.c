/*
 * asi.c
 *
 *  Created on: May 17, 2025
 *      Author: Сашенька
 */


#include "asi.h"
#include "asi_typedefs.h"
#include "asi_rx/asi_rx.h"
#include "asi_tx/asi_tx.h"

extern void ASI_init() {

}

extern void ASI_write(uint8_t* data, uint16_t size) {
	TX_send(data, size);
}

extern void ASI_read(uint8_t* data, uint16_t size) {
	RX_read(data, size);
}

extern void ASI_transmitterTimerCallback(void) {
	TX_timerCompleteCallback();
}

extern void ASI_receiverTimerCallback(void) {
	RX_runStopStateOnTimerFullPeriodEvent();
	RX_runReceivingStateOnTimerFullPeriodEvent();
	RX_runWaitStateOnTimerHalfPeriodEvent();
}

extern void ASI_receiverFallingEdgeDataPinCallback(void) {
	RX_runSyncFirstStepOnFallingEdgeEvent();
}

extern void ASI_receiverRisingEdgeDataPinCallback(void) {
	RX_runSyncSecondStateOnRisingEdgeEvent();
}
