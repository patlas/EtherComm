#ifndef _CIRCBUF_H_
#define _CIRCBUF_H_

#include <stdint.h>
#include <string.h>
#include "TimeoutTimer.h"



#define TX_BUFFER_SIZE 10
#define UART_BUFFER_SIZE 8
#define CIRC_BUFF_SIZE 100
#define TX_TIMEOUT 10 //timeout incoming data in ms 
#define ABSIZE 10 //A and B buffer size

#define UART_READ_DATA_REG UART0->D
#define CLEAR_TIMEOUT_TIMER 

extern uint8_t buffA[ABSIZE];
extern uint8_t buffB[ABSIZE];

extern uint8_t tx_buffer[TX_BUFFER_SIZE]; //bd 100

typedef struct {
	uint8_t *write_ptr;
	uint8_t *read_ptr;
	uint8_t write_size;
	uint8_t read_size;
	uint8_t AorB;
} HBuffer;


typedef struct {
	uint8_t *buff_start;
	uint8_t *read_ptr;
	uint16_t buff_cap;
	uint16_t size;
	uint16_t windex;
} CBuffer;
	


extern HBuffer HalfBuffTx;
extern HBuffer HalfBuffRx;

extern CBuffer CircBuffTx;
extern CBuffer CircBuffRx;
extern uint8_t CTxBuff[CIRC_BUFF_SIZE];
extern uint8_t CRxBuff[CIRC_BUFF_SIZE];

extern bool tx_buff_ready_flag;
extern uint16_t tx_reduced_size;
extern uint16_t uart_reduced_size;

void DMA_startTX(void);

void ReadUARTdata(void);
void HalfBuffInit(void);


void DMA_startTX2(uint8_t *src, uint8_t *dst, uint16_t size);
void CircBuffInit(void);
bool CircBuffRead(CBuffer *bptr, uint8_t *dst_buff,uint16_t len );
bool CircBuffWrite(CBuffer *bptr, uint8_t *data );
bool CircBuffRead4Uart(CBuffer *bptr, uint8_t *dst_buff, uint16_t len );





#endif
