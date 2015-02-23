#ifndef __TIMEOUTTIMER_H__
#define __TIMEOUTTIMER_H__

#include <stdint.h>
#include "fsl_pit_driver.h"
#include "CircBuf.h"

#define PIT_INSTANCE 0


#include "lwip/tcp.h"
extern volatile uint8_t tx_flag;
extern volatile uint8_t timeout_flag;



void TO_Timer_init(uint8_t timeout_ms);
void TO_Timer_reset(void); 
void TO_Timer_stop(void); 
void TO_Timer_callbackIRQ(void);
#endif
