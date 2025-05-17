/*
 * asi_typedefs.h
 *
 *  Created on: May 17, 2025
 *      Author: Сашенька
 */

#ifndef ASI_ASI_TYPEDEFS_H_
#define ASI_ASI_TYPEDEFS_H_

/* TRANSMITTER */

typedef struct {

}tx_t;

/* RECEIVER */
typedef enum
{
	idle,
	preparing,
	ready,
	syncStep1,
	syncStep2,
	receiving,
	stop,
	finish,
	states
} rxState_t;

typedef struct {
	rxState_t state;
	uint8_t*  data;
	uint8_t   size;
	uint8_t   currentBit;
	uint8_t   byte;
	uint8_t   currentByte;
	uint32_t  arr;
	uint32_t  period;
}rx_t;

#endif /* ASI_ASI_TYPEDEFS_H_ */
