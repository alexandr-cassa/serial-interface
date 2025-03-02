///*
// * rx.c
// *
// *  Created on: Feb 10, 2025
// *      Author: Сашенька
// */
//
//#include "rx.h"
//
//#define BYTE_SIZE (8u)
//
//static signal getSignal;
//
//static enum
//{
//	idle,
//	receiving
//} rxState;
//
//static struct
//{
//	signal signalState;
//	rxState state;
//	uint8_t* data;
//	uint8_t size;
//	uint8_t currentBit;
//	uint8_t byte;
//	uint8_t currentByte;
//} rx;
//
//static inline void reset(void);
//static inline void readBitIntoByte(void);
//static inline void shiftByteOrPutItInBufferIfCompleted(void);
//static inline void resetRxIfBufferIsFull(void);
//
//void RX_init(signal source)
//{
//	getSignal = source;
//}
//
//
//void RX_start(uint8_t* data, uint8_t size)
//{
//	rx.data = data;
//	rx.size = size;
//	rx.state = receiving;
//}
//
//void RX_callback(void)
//{
//	if(idle != rx.state)
//	{
//		readBitIntoByte();
//		shiftByteOrPutItInBufferIfCompleted();
//		resetRxIfBufferIsFull();
//	}
//}
//
//static inline void reset(void)
//{
//	rx.currentBit = 0;
//	rx.state = idle;
//	rx.byte = 0;
//	rx.currentByte = 0;
//}
//
//static inline void readBitIntoByte(void)
//{
//	rx.byte |= rx.signalState() & (1u);
//}
//
//static inline void shiftByteOrPutItInBufferIfCompleted(void)
//{
//	if (BYTE_SIZE >= rx.currentBit)
//	{
//		rx.data[rx.currentByte++] = rx.byte;
//		rx.byte = 0;
//		rx.currentBit = 0;
//	}
//	else
//	{
//		rx.byte >>= 1;
//		rx.currentBit++;
//	}
//}
//
//static inline void resetRxIfBufferIsFull(void)
//{
//	if (rx.currentByte >= rx.size)
//	{
//		reset();
//	}
//}
