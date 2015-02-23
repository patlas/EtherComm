#include "TimeoutTimer.h"

void TO_Timer_init(uint8_t timeout_ms){
	
	pit_user_config_t config = {
		.isInterruptEnabled = true,
		.isTimerChained = false,
		.periodUs = 1000*timeout_ms,
	};
	
	PIT_DRV_Init ( PIT_INSTANCE, true );
	PIT_DRV_InitChannel ( PIT_INSTANCE, 0, &config );
	PIT_DRV_InstallCallback ( PIT_INSTANCE, 0, TO_Timer_callbackIRQ );
	
}
void TO_Timer_reset(void){
	
	PIT_DRV_StartTimer ( PIT_INSTANCE, 0 );
	
}
void TO_Timer_stop(void){
	
	PIT_DRV_StopTimer ( PIT_INSTANCE, 0 );
	
}

void TO_Timer_callbackIRQ(void){
	
	if( CircBuffTx.size >0 )
		timeout_flag = 1;
	else
		TO_Timer_stop();
		
}
