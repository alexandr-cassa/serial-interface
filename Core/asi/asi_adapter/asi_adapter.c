/*
 * asi_adapter.c
 *
 *  Created on: May 17, 2025
 *      Author: Сашенька
 */

#include "asi_adapter.h"

static void switchDataPinMode(uint32_t mode);

uint8_t readBit(void) {
	return !HAL_GPIO_ReadPin (
		RX_DATA_GPIO_Port,
		RX_DATA_Pin
	);
}
void writeBit(uint8_t bit) {
	HAL_GPIO_WritePin (
		DATA_GPIO_Port,
		DATA_Pin,
		(bit != 0)
	);
}

void setReceiverDataPinStateToGpioInput(void) {
	switchDataPinMode(GPIO_MODE_INPUT);
}

void setReceiverDataPinStateToEXTI(void) {
	switchDataPinMode(GPIO_MODE_IT_RISING_FALLING);
}

uint32_t getReceiverTimerCntValue(void) {
	return TIM2->CNT;
}
void setReceiverTimerCntValue(uint32_t val) {
	TIM2->CNT = val;
}

uint32_t getReceiverTimerArrValue(void) {
	return TIM2->ARR;
}

void setReceiverTimerArrValue(uint32_t val) {
	TIM2->ARR = val;
}

void receiverTimerBaseStart(void) {
	HAL_TIM_Base_Start(htim2);
}

void receiverTimerBaseStop(void) {
	HAL_TIM_Base_Stop(htim2);
}

void receiverTimerBaseStartIT(void) {
	HAL_TIM_Base_Start_IT(&htim2);
}

void receiverTimerBaseStopIT(void) {
	HAL_TIM_Base_Stop_IT(&htim2);
}

static void switchDataPinMode(uint32_t mode) {
	HAL_GPIO_DeInit(RX_DATA_GPIO_Port, RX_DATA_Pin);

	GPIO_InitTypeDef initStruct = {
		.Pin = RX_DATA_Pin,
		.Mode = mode,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_LOW
	};
	HAL_GPIO_Init(RX_DATA_GPIO_Port, &initStruct);
}
