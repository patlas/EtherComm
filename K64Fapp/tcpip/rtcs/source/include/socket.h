#ifndef __socket_h__
#define __socket_h__
/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
* Copyright 2004-2008 Embedded Access Inc.
* Copyright 1989-2008 ARC International
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale MQX RTOS License
* distributed with this Material.
* See the MQX_RTOS_LICENSE file distributed for more details.
*
* Brief License Summary:
* This software is provided in source form for you to use free of charge,
* but it is not open source software. You are allowed to use this software
* but you cannot redistribute it or derivative works of it in source form.
* The software may be used only in connection with a product containing
* a Freescale microprocessor, microcontroller, or digital signal processor.
* See license agreement file for full license terms including other
* restrictions.
*****************************************************************************
*
* Comments:
*
*   Constants and prototype declarations which are internal
*   to the SOCKET module.
*
*
*END************************************************************************/
#include "rtcstime.h"

/*
** Connectionless-mode socket states
*/
#define SOCKSTATE_DGRAM_GROUND      0
#define SOCKSTATE_DGRAM_BOUND       1
#define SOCKSTATE_DGRAM_OPEN        2

/*
** Connection-mode socket states
*/
#define SOCKSTATE_STREAM_GROUND     0
#define SOCKSTATE_STREAM_BOUND      1
#define SOCKSTATE_STREAM_LISTENING  2
#define SOCKSTATE_STREAM_CONNECTED  3


/*
**  socket-specific structures
*/
/* Select functions */
typedef struct sock_select_parm {
   TCPIP_PARM                          COMMON;
   struct sock_select_parm             *NEXT;
   struct sock_select_parm            **PREV;
   TCPIP_EVENT                         EXPIRE;
   _rtcs_taskid                        owner;
   rtcs_fd_set *restrict                    readfds;
   rtcs_fd_set *restrict                    writefds;
   uint32_t                            sock_count;
   uint32_t                            timeout;
   uint32_t                            sock;
} SOCK_SELECT_PARM, * SOCK_SELECT_PARM_PTR;

#define SOCK_select_signal(a, b)    _SOCK_select_signal(a, b, 0)

extern void    _SOCK_select_signal   (uint32_t, uint16_t, uint32_t);

typedef struct socket_config_struct {

   bool                       INITIALIZED;
   uint32_t                       CURRENT_SOCKETS;
   void                         *SOCKET_HEAD;
   void                         *SOCKET_TAIL;
   struct sock_select_parm       *SELECT_HEAD;
   void                         *SELECT_TIME_HEAD;    /* not used */
   _rtcs_mutex                   SOCK_MUTEX;

} SOCKET_CONFIG_STRUCT, * SOCKET_CONFIG_STRUCT_PTR;


/****************************************************
** extern statements for socket procedures          *
*****************************************************/

extern SOCKET_STRUCT_PTR   SOCK_Get_sock_struct
(
   RTCS_SOCKET_CALL_STRUCT_PTR   type,
   _rtcs_taskid                  owner
);
extern void SOCK_Free_sock_struct
(
   SOCKET_STRUCT_PTR          socket_ptr
);

#if RTCSCFG_BACKWARD_COMPATIBILITY_RTCSSELECT==1
extern bool SOCK_Remove_owner( SOCKET_STRUCT_PTR, void *);
extern bool SOCK_Is_owner( SOCKET_STRUCT_PTR, void *);
extern bool SOCK_Add_owner( SOCKET_STRUCT_PTR, void *);
#endif


#endif
/* EOF */
