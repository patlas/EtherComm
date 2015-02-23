#include "CircBuf.h"
#include "MK64F12.h"



void DMA_startTX(void){
	for (unsigned int x = 0; x < ABSIZE; x++)
		tx_buffer[x] = *(HalfBuffTx.read_ptr + x);
}

void DMA_startTX2(uint8_t *src, uint8_t *dst, uint16_t size){
	for (unsigned int x = 0; x < size; x++)
		dst[x] = src[x];
}


void HalfBuffInit(void){

	HalfBuffTx.AorB = 'A';
	HalfBuffTx.write_ptr = buffA;
	HalfBuffTx.read_ptr = NULL;
	HalfBuffTx.read_size = 0;
	HalfBuffTx.write_size = 0;

}

void ReadUARTdata(void){

	TO_Timer_reset();	
	CircBuffWrite(&CircBuffTx, &UART_READ_DATA_REG );

}



void CircBuffInit(void){
	
	CircBuffTx.buff_start = &CTxBuff[0];
	CircBuffTx.read_ptr = &CTxBuff[0];
	CircBuffTx.buff_cap = CIRC_BUFF_SIZE;
	CircBuffTx.size = 0;
	CircBuffTx.windex = 0;
	
	CircBuffRx.buff_start = &CRxBuff[0];
	CircBuffRx.read_ptr = &CRxBuff[0];
	CircBuffRx.buff_cap = CIRC_BUFF_SIZE;
	CircBuffRx.size = 0;
	CircBuffRx.windex = 0;
	
}




bool CircBuffRead(CBuffer *bptr, uint8_t *dst_buff, uint16_t len ){ //len to stala wartosc np 10

	
	if(tx_buff_ready_flag == true){
		tx_buff_ready_flag = false;
		if( bptr->size >0  && timeout_flag==1 ){
			
			if(  bptr->size < len){
				DMA_startTX2(bptr->read_ptr, dst_buff,  bptr->size);
				bptr->windex += (len - bptr->size);
				tx_reduced_size = bptr->size;
				bptr->size = 0;
				bptr->read_ptr += len;
			}
			else{
				DMA_startTX2(bptr->read_ptr, dst_buff,  len);
				bptr->size -= len;
				bptr->read_ptr += len;
			}
			
			if(bptr->read_ptr >= (bptr->buff_start + bptr->buff_cap) ) bptr->read_ptr = bptr->buff_start;
			timeout_flag=0;
			return true;
		}
		else if( bptr->size >=len ){
			DMA_startTX2(bptr->read_ptr, dst_buff,  len);
			bptr->size -= len;
			bptr->read_ptr += len;
			if(bptr->read_ptr >= (bptr->buff_start + bptr->buff_cap) ) bptr->read_ptr = bptr->buff_start;
			return true;
			
		}
		else 
			return false;
	}
	else
		return false;
}


bool CircBuffWrite(CBuffer *bptr, uint8_t *data){ 
	
	int32_t asize = bptr->buff_cap - bptr->size;
	if( asize>0 ){
			
			if( bptr->windex == (bptr->buff_cap /*-1*/) )
				bptr->windex = 0;
			
			bptr->buff_start[bptr->windex] = *data;
			bptr->windex++;

			bptr->size++;
			
			return true;
	}
	else
		return false;
}



bool CircBuffRead4Uart(CBuffer *bptr, uint8_t *dst_buff, uint16_t len ){ //len to stala wartosc np 10


		if( bptr->size >0  && timeout_flag==3 /*timeout flag zrobic wlasne albo wyeliminowac*/ ){
			
			if(  bptr->size < len){
				DMA_startTX2(bptr->read_ptr, dst_buff,  bptr->size);
				bptr->windex += (len - bptr->size);
				uart_reduced_size = bptr->size;
				bptr->size = 0;
				bptr->read_ptr += len;
			}
			else{
				DMA_startTX2(bptr->read_ptr, dst_buff,  len);
				bptr->size -= len;
				bptr->read_ptr += len;
			}
			
			if(bptr->read_ptr >= (bptr->buff_start + bptr->buff_cap) ) bptr->read_ptr = bptr->buff_start;
			timeout_flag=0;
			return true;
		}
		else if( bptr->size >=len ){
			DMA_startTX2(bptr->read_ptr, dst_buff,  len);
			bptr->size -= len;
			bptr->read_ptr += len;
			if(bptr->read_ptr >= (bptr->buff_start + bptr->buff_cap) ) bptr->read_ptr = bptr->buff_start;
			return true;
			
		}
		else 
			return false;

}