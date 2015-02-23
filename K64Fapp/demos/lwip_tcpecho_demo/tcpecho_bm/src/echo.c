/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of and a contribution to the lwIP TCP/IP stack.
 *
 * Credits go to Adam Dunkels (and the current maintainers) of this software.
 *
 * Christiaan Simons rewrote this file to get a more stable echo example.
 */

/**
 * @file
 * TCP echo server example using raw API.
 *
 * Echos all bytes sent by connecting client,
 * and passively closes when client is done.
 *
 */

//#include "MK64F12.h"

#include "lwip/opt.h"
#include "CircBuf.h"
#include "TimeoutTimer.h"

#if LWIP_TCP
#include <stdio.h>
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwip/mem.h"
#include "lwip/raw.h"
#include "lwip/icmp.h"
#include "lwip/netif.h"
#include "lwip/sys.h"
#include "lwip/timers.h"
#include "lwip/inet_chksum.h"
#include "lwip/init.h"
#include "netif/etharp.h"

#include "fsl_clock_manager.h"
#include "fsl_uart_driver.h"
#include "fsl_device_registers.h"
#include "fsl_port_hal.h"
#include "fsl_sim_hal.h"
#include "fsl_os_abstraction.h"
#include "ethernetif.h"
#include "board.h"
#include "fsl_pit_driver.h"
#include "fsl_uart_common.h"

//#include "api.h"

uint32_t portBaseAddr[] = PORT_BASE_ADDRS;
uint32_t simBaseAddr[] = SIM_BASE_ADDRS;


uint8_t buffA[ABSIZE];
uint8_t buffB[ABSIZE];
HBuffer HalfBuffTx;
HBuffer HalfBuffRx;
uint8_t tx_buffer[TX_BUFFER_SIZE];
uint8_t tx_uart[UART_BUFFER_SIZE];

uint8_t CTxBuff[CIRC_BUFF_SIZE];
uint8_t CRxBuff[CIRC_BUFF_SIZE];
CBuffer CircBuffTx;
CBuffer CircBuffRx;


static struct tcp_pcb *etherComm_pcb;

volatile uint8_t tx_flag=0;
volatile uint8_t timeout_flag=0;
bool tx_buff_ready_flag = true;
uint16_t tx_reduced_size = TX_BUFFER_SIZE;
uint16_t uart_reduced_size = TX_BUFFER_SIZE;
uint8_t uart_ready = 0;
enum etherComm_states
{
  ES_NONE = 0,
  ES_ACCEPTED,
  ES_RECEIVED,
  ES_CLOSING
};

struct etherComm_state
{
  u8_t state;
  u8_t retries;
  struct tcp_pcb *pcb;
  /* pbuf (chain) to recycle */
  struct pbuf *p;
};

err_t etherComm_accept(void *arg, struct tcp_pcb *newpcb, err_t err);
err_t etherComm_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
void etherComm_error(void *arg, err_t err);
err_t etherComm_poll(void *arg, struct tcp_pcb *tpcb);
err_t etherComm_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
void etherComm_send(struct tcp_pcb *tpcb, struct etherComm_state *es);
void etherComm_close(struct tcp_pcb *tpcb, struct etherComm_state *es);

void TCP_UART_send(struct tcp_pcb *tpcb, struct etherComm_state *es);
err_t TCP_UART_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
uart_status_t UART_rcv_callback(uint8_t * rxByte, void * param);

void connection_init(void);

void ReadInPoll(struct tcp_pcb *tpcb);


void
etherComm_init(void)
{
  etherComm_pcb = tcp_new();
  if (etherComm_pcb != NULL)
  {
    err_t err;

    err = tcp_bind(etherComm_pcb, IP_ADDR_ANY, 7);  
    if (err == ERR_OK)
    {
      etherComm_pcb = tcp_listen(etherComm_pcb);
      tcp_accept(etherComm_pcb, etherComm_accept);
			
    }
    else 
    {
      /* abort? output diagnostic? */
    }
  }
  else
  {
    /* abort? output diagnostic? */
  }
}


err_t
etherComm_accept(void *arg, struct tcp_pcb *newpcb, err_t err) // accept incomming connection
{
  err_t ret_err;
  struct etherComm_state *es;

  LWIP_UNUSED_ARG(arg);
  LWIP_UNUSED_ARG(err);

  /* Unless this pcb should have NORMAL priority, set its priority now.
     When running out of pcbs, low priority pcbs can be aborted to create
     new pcbs of higher priority. */
  tcp_setprio(newpcb, TCP_PRIO_MIN);

  es = (struct etherComm_state *)mem_malloc(sizeof(struct etherComm_state));
  if (es != NULL)
  {
    es->state = ES_ACCEPTED;
    es->pcb = newpcb;
    es->retries = 0;
    es->p = NULL;
    /* pass newly allocated es to our callbacks */
    tcp_arg(newpcb, es);
    tcp_recv(newpcb, etherComm_recv);
    tcp_err(newpcb, etherComm_error);
    tcp_poll(newpcb, etherComm_poll, 0/*0*/);
    ret_err = ERR_OK;
  }
  else
  {
    ret_err = ERR_MEM;
  }
  return ret_err;  
}

err_t
etherComm_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) // receiving incomming data
{
  struct etherComm_state *es;
  err_t ret_err;

  LWIP_ASSERT("arg != NULL",arg != NULL);
  es = (struct etherComm_state *)arg;
  if (p == NULL)
  {

    es->state = ES_CLOSING;
    if(es->p == NULL)
    {

       etherComm_close(tpcb, es);
    }
    else
    {

    }
    ret_err = ERR_OK;
  }
  else if(err != ERR_OK)
  {

    if (p != NULL)
    {
      es->p = NULL;
      pbuf_free(p);
    }
    ret_err = err;
  }
  else if(es->state == ES_ACCEPTED)
  {

    es->state = ES_RECEIVED;

    es->p = p;

		uint8_t tempbuff[4];
	
		//copy uart configuration frame
		memcpy(tempbuff,es->p->payload,es->p->len);


		uart_state_t uartState;
		//enable uart interface  being modified
		UART0-> C2 &= ~UART_C2_TE_MASK & ~UART_C2_RE_MASK;
	
		uint32_t baudRate;
		// decode frame to set baudrate
		switch(tempbuff[0]){
			case 0:
				baudRate = 110;
			break;
		
			case 1:
				baudRate = 150;
			break;
			
			case 2:
				baudRate = 300;
			break;
			
			case 3:
				baudRate = 1200;
			break;
			
			case 4:
				baudRate = 2400;
			break;
			
			case 5:
				baudRate = 4800;
			break;
			
			case 6:
				baudRate = 9600;
			break;
			
			case 7:
				baudRate = 19200;
			break;
			
			case 8:
				baudRate = 38400;
			break;
			
			case 9:
				baudRate = 57600;
			break;
			
			case 10:
				baudRate = 115200;
			break;
			
			case 11:
				baudRate = 230400;
			break;
			
			case 12:
				baudRate = 460800;
			break;
			
			case 13:
				baudRate = 921600;
			break;
		};
		 
		uint32_t uartSourceClock = CLOCK_SYS_GetUartFreq(BOARD_DEBUG_UART_INSTANCE);
		UART_HAL_SetBaudRate(g_uartBaseAddr[BOARD_DEBUG_UART_INSTANCE], uartSourceClock, baudRate);
		// decode frame to set data length 
		switch(tempbuff[1]){
			case 3:
				UART0->C1 &= ~UART_C1_M_MASK; //8bit data
			break;
			
			case 2:
				UART0->C1 |= UART_C1_M_MASK; // 9bits data
			break;
		};
		
		// decode frame to set parity type
		switch(tempbuff[2]){
			case 0:
				UART0->C1 &= ~UART_C1_PE_MASK; // parity none
			break;
			
			case 2:
				UART0->C1 |= UART_C1_PE_MASK;
				UART0->C1 &= ~UART_C1_PT_MASK; // even parity 
			break;
			
			case 1:
				UART0->C1 |= UART_C1_PE_MASK;
				UART0->C1 |= UART_C1_PT_MASK; // odd parity
			break;
		};
		// setting stop bits
		switch(tempbuff[2]){
			case 0:
				UART0->BDH &= ~UART_BDH_SBNS_MASK;
			break;
			case 1:
				UART0->BDH |= UART_BDH_SBNS_MASK; //2 stop bits, &= ~UART_SBNS => 1 stop bitCountPerChar
			break;
		}
		UART0-> C2 |= UART_C2_TE_MASK | UART_C2_RE_MASK;
		
		uart_ready = 1;
			
    ret_err = ERR_OK;
  }
  else if (es->state == ES_RECEIVED)
  {
    /* read some more data */
    if(es->p == NULL)
    {
      es->p = p;
      tcp_sent(tpcb, TCP_UART_sent);
			
			// if data is being received strat sendint via uart in non blocking state
			UART_DRV_SendData(BOARD_DEBUG_UART_INSTANCE, es->p->payload,es->p->len);
			TCP_UART_send(tpcb, es);
    }
    else
    {
      struct pbuf *ptr;

      /* chain pbufs to the end of what we recv'ed previously  */
      ptr = es->p;
      pbuf_chain(ptr,p);
    }
    ret_err = ERR_OK;
  }
  else if(es->state == ES_CLOSING)
  {
    /* odd case, remote side closing twice, trash data */
    tcp_recved(tpcb, p->tot_len);
    es->p = NULL;
    pbuf_free(p);
    ret_err = ERR_OK;
  }
  else
  {
    /* unkown es->state, trash data  */
    tcp_recved(tpcb, p->tot_len);
    es->p = NULL;
    pbuf_free(p);
    ret_err = ERR_OK;
  }
  return ret_err;
}

void
etherComm_error(void *arg, err_t err)
{
  struct etherComm_state *es;

  LWIP_UNUSED_ARG(err);

  es = (struct etherComm_state *)arg;
  if (es != NULL)
  {
    mem_free(es);
  }
}

err_t
etherComm_poll(void *arg, struct tcp_pcb *tpcb)
{
  err_t ret_err;
  struct etherComm_state *es;

	
	ReadInPoll(tpcb);
	
  es = (struct etherComm_state *)arg;
  if (es != NULL)
  {
    if (es->p != NULL)
    {
      /* there is a remaining pbuf (chain)  */
      tcp_sent(tpcb, etherComm_sent);
      etherComm_send(tpcb, es);
    }
    else
    {
      /* no remaining pbuf (chain)  */
      if(es->state == ES_CLOSING)
      {
        // stop blinking led if connection closed
				GPIO_DRV_TogglePinOutput(BOARD_GPIO_LED_RED);
      }
    }
    ret_err = ERR_OK;
  }
  else
  {
    /* nothing to be done */
    tcp_abort(tpcb);
    ret_err = ERR_ABRT;
  }
	
	//blinking led if connection is established
	GPIO_DRV_TogglePinOutput(BOARD_GPIO_LED_RED);
  return ret_err;
}

err_t
etherComm_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
  struct etherComm_state *es;

  LWIP_UNUSED_ARG(len);

  es = (struct etherComm_state *)arg;
  es->retries = 0;
  
  if(es->p != NULL)
  {
    /* still got pbufs to send */
    tcp_sent(tpcb, etherComm_sent);
    etherComm_send(tpcb, es);
  }
  else
  {
    /* no more pbufs to send */
    if(es->state == ES_CLOSING)
    {
      etherComm_close(tpcb, es);
    }
  }
  return ERR_OK;
}

void
etherComm_send(struct tcp_pcb *tpcb, struct etherComm_state *es)
{
  struct pbuf *ptr;
  err_t wr_err = ERR_OK;
 
  while ((wr_err == ERR_OK) &&
         (es->p != NULL) && 
         (es->p->len <= tcp_sndbuf(tpcb)))
  {
  ptr = es->p;

  /* enqueue data for transmission */
  wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);
  if (wr_err == ERR_OK)
  {
     u16_t plen;
      u8_t freed;

     plen = ptr->len;
     /* continue with next pbuf in chain (if any) */
     es->p = ptr->next;
     if(es->p != NULL)
     {
       /* new reference! */
       pbuf_ref(es->p);
     }
     /* chop first pbuf from chain */
      do
      {
        /* try hard to free pbuf */
        freed = pbuf_free(ptr);
      }
      while(freed == 0);
     /* we can read more data now */
     tcp_recved(tpcb, plen);
   }
   else if(wr_err == ERR_MEM)
   {
      /* we are low on memory, try later / harder, defer to poll */
     es->p = ptr;
   }
   else
   {
     /* other problem ?? */
   }
  }
}

void TCP_UART_send(struct tcp_pcb *tpcb, struct etherComm_state *es)
{
	int i;
	uint8_t dat[8];
	
  struct pbuf *ptr;
  err_t wr_err = ERR_OK;
 
  while ((wr_err == ERR_OK) &&
         (es->p != NULL) && 
         (es->p->len <= tcp_sndbuf(tpcb)))
  {
  ptr = es->p;

		memcpy(dat,es->p->payload,es->p->len);
		for(i=0;i<es->p->len; i++){
			CircBuffWrite(&CircBuffRx, &dat[i]);
		}
		wr_err = ERR_OK;
  if (wr_err == ERR_OK)
  {
     u16_t plen;
      u8_t freed;

     plen = ptr->len;
     /* continue with next pbuf in chain (if any) */
     es->p = ptr->next;
     if(es->p != NULL)
     {
       /* new reference! */
       pbuf_ref(es->p);
     }
     /* chop first pbuf from chain */
      do
      {
        /* try hard to free pbuf */
        freed = pbuf_free(ptr);
      }
      while(freed == 0);
     /* we can read more data now */
     tcp_recved(tpcb, plen);
   }
   else if(wr_err == ERR_MEM)
   {
      /* we are low on memory, try later / harder, defer to poll */
     es->p = ptr;
   }
   else
   {
     /* other problem ?? */
   }
  }
}


err_t
TCP_UART_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
  struct etherComm_state *es;

  LWIP_UNUSED_ARG(len);

  es = (struct etherComm_state *)arg;
  es->retries = 0;
  
  if(es->p != NULL)
  {
    /* still got pbufs to send */
    tcp_sent(tpcb, TCP_UART_sent);
    TCP_UART_send(tpcb, es);
  }
  else
  {
    /* no more pbufs to send */
    if(es->state == ES_CLOSING)
    {
      etherComm_close(tpcb, es);
    }
  }
  return ERR_OK;
}



void
etherComm_close(struct tcp_pcb *tpcb, struct etherComm_state *es)
{
  tcp_arg(tpcb, NULL);
  tcp_sent(tpcb, NULL);
  tcp_recv(tpcb, NULL);
  tcp_err(tpcb, NULL);
  tcp_poll(tpcb, NULL, 0);
  
  if (es != NULL)
  {
    mem_free(es);
  }  
  tcp_close(tpcb);
}

#endif /* LWIP_TCP */



/*Add by myself*/
void init_hardware(void)
{
    /* Open uart module for debug */
    hardware_init();

    dbg_uart_init();

    /* Open ENET clock gate*/
    CLOCK_SYS_EnableEnetClock(0);
    /* Select PTP timer outclk*/
    CLOCK_SYS_SetSource(kClockTimeSrc, 2);

    /* Disable the mpu*/
    BW_MPU_CESR_VLD(MPU_BASE, 0);
}





void ReadInPoll(struct tcp_pcb *tpcb){
	
	if(tx_flag==1)
	{		
		tcp_write(tpcb,&tx_buffer[0],tx_reduced_size,1);
		tx_reduced_size = TX_BUFFER_SIZE;
		tx_flag = 0;
	}
	tx_buff_ready_flag = true;
	
	
}



int main(void)
{

	
  struct netif fsl_netif0;
  ip_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
  init_hardware();
	
	/***********************************************/
	uart_user_config_t uartConfig;
	uartConfig.baudRate = 9600;//115200;
	uartConfig.bitCountPerChar = kUart8BitsPerChar;
	uartConfig.parityMode = kUartParityDisabled;
	uartConfig.stopBitCount = kUartOneStopBit;
	uart_state_t uartState;
	/**********************************************/
	
	
  OSA_Init();
	UART_DRV_Init(BOARD_DEBUG_UART_INSTANCE, &uartState, &uartConfig);
	
	
  lwip_init();
  IP4_ADDR(&fsl_netif0_ipaddr, 192,168,2,102); // IP ADDR of device
  IP4_ADDR(&fsl_netif0_netmask, 255,255,255,0); // SUBNET mask
  IP4_ADDR(&fsl_netif0_gw, 192,168,2,100); // GATEWAY addr
  netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, NULL, ethernetif_init, ethernet_input);
  netif_set_default(&fsl_netif0);
  netif_set_up(&fsl_netif0);
	
	CircBuffInit();
  etherComm_init();

	
	TO_Timer_init(10); //10ms data timeout ewentualnie przeliczac ze wzgledu na baud rate -> mozna wewnatrz funkcji
	
	
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PTB->PDDR |= 1<<22 | 1<<21;
	LED2_EN;
	LED3_EN;
	LED2_ON;
	LED3_ON;
	
	
	
#if !ENET_RECEIVE_ALL_INTERRUPT
	uint32_t devNumber = 0; 
	enet_dev_if_t * enetIfPtr;
#if LWIP_HAVE_LOOPIF
	devNumber = fsl_netif0.num - 1;
#else
	devNumber = fsl_netif0.num;
#endif
	enetIfPtr = (enet_dev_if_t *)&enetDevIf[devNumber];
#endif
  while(1)
  {
		
		
#if !ENET_RECEIVE_ALL_INTERRUPT
    ENET_receive(enetIfPtr);
#endif

		if( (UART0->S1 & UART_S1_RDRF_MASK) == UART_S1_RDRF_MASK ){
			ReadUARTdata();
	}
		
	
	if(CircBuffRead(&CircBuffTx, &tx_buffer[0], TX_BUFFER_SIZE )== true)
		tx_flag=1;
	

		if(CircBuffRead4Uart(&CircBuffRx, &tx_uart[0], UART_BUFFER_SIZE ) == true){
		
		}
    sys_check_timeouts();

  }
	
  
}



