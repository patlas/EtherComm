/*HEADER**********************************************************************
*
* Copyright 2013 Freescale Semiconductor, Inc.
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
*   This file contains the HTTPSRV implementation.
*
*
*END************************************************************************/

#include "httpsrv.h"
#include "httpsrv_supp.h"
#include "httpsrv_prv.h"
#include <string.h>

#define HTTPSRV_SERVER_TASK_NAME "HTTP server"

/*
** Function for starting the HTTP server 
**
** IN:
**      HTTPSRV_PARAM_STRUCT*   params - server parameters (port, ip, index page etc.)
**
** OUT:
**      none
**
** Return Value: 
**      uint32_t      server handle if successful, NULL otherwise
*/
uint32_t HTTPSRV_init(HTTPSRV_PARAM_STRUCT *params)
{
    HTTPSRV_SERVER_TASK_PARAM server_params;

    server_params.params = params;

    /* Server must run with lower priority than TCP/IP task. */
    if (params->server_prio == 0)
    {
        params->server_prio = HTTPSRVCFG_DEF_SERVER_PRIO;
    }
    else if (params->server_prio < _RTCSTASK_priority)
    {
        return(0);
    }
    
    /* Run server task. */
    if (RTCS_task_create(HTTPSRV_SERVER_TASK_NAME, params->server_prio, HTTPSRV_SERVER_STACK_SIZE, httpsrv_server_task, &server_params) != RTCS_OK)
    {
        HTTPSRV_release(server_params.handle);
        return(0);
    }

    return(server_params.handle);
}

/*
** Function for releasing/stopping HTTP server 
**
** IN:
**      uint32_t       server_h - server handle
**
** OUT:
**      none
**
** Return Value: 
**      uint32_t      error code. HTTPSRV_OK if everything went right, positive number otherwise
*/
uint32_t HTTPSRV_release(uint32_t server_h)
{
    HTTPSRV_STRUCT* server = (void *) server_h;

    /* Shutdown server task and wait for its termination. */
    if (server->sock_v4)
    {
        shutdown(server->sock_v4, FLAG_ABORT_CONNECTION);
    }
    if (server->sock_v6)
    {
        shutdown(server->sock_v6, FLAG_ABORT_CONNECTION);
    }
    
    while(server->server_tid)
    {
        _sched_yield();
    }
    return(RTCS_OK);
}

/*
** Write data to client from CGI script
**
** IN:
**      HTTPSRV_CGI_RES_STRUCT* response - CGI response structure used for forming response
**
** OUT:
**      none
**
** Return Value:
**      uint_32 - Number of bytes written
*/
uint32_t HTTPSRV_cgi_write(HTTPSRV_CGI_RES_STRUCT* response)
{
    HTTPSRV_SESSION_STRUCT* session = (HTTPSRV_SESSION_STRUCT*) response->ses_handle;
    uint32_t retval = 0;

    if (session == NULL)
    {
        return(0);
    }

    if (session->response.hdrsent == 0)
    {
        session->response.status_code = response->status_code;
        session->response.content_type = response->content_type;
        session->response.len = response->content_length;
        /* 
        ** If there is no content length we have to disable keep alive.
        ** Otherwise we would have to wait for session timeout.
        */
        if (session->response.len == 0)
        {
            session->keep_alive = 0;
        }
        httpsrv_sendhdr(session, response->content_length, 1);
    }
    if ((response->data != NULL) && (response->data_length))
    {
        retval = httpsrv_write(session, response->data, response->data_length);
    }
    session->time = RTCS_time_get();
    return(retval);
}

/*
** Read data from client to CGI script
**
** IN:
**      uint32_t ses_handle - handle to session used for reading
**      char*   buffer - user buffer to read data to
**      uint32_t length - size of buffer in bytes
**
** OUT:
**      none
**
** Return Value:
**      uint32_t - Number of bytes read
*/
uint32_t HTTPSRV_cgi_read(uint32_t ses_handle, char* buffer, uint32_t length)
{
    HTTPSRV_SESSION_STRUCT* session = (HTTPSRV_SESSION_STRUCT*) ses_handle;
    uint32_t retval;

    retval = httpsrv_read(session, buffer, length);

    if (retval > 0)
    {
        session->request.content_length -= retval;
    }
    session->time = RTCS_time_get();
    return(retval);
}

/*
** Write data to client from server side include
**
** IN:
**      uint32_t ses_handle - session foe writing
**      char*   data - user data to write
**      uint32_t length - size of data in bytes
**
** OUT:
**      none
**
** Return Value:
**      uint32_t - Number of bytes written
*/
uint32_t HTTPSRV_ssi_write(uint32_t ses_handle, char* data, uint32_t length)
{
    HTTPSRV_SESSION_STRUCT* session = (HTTPSRV_SESSION_STRUCT*) ses_handle;
    uint32_t retval = 0;

    if ((session != NULL) && (data != NULL) && (length))
    {
        retval = httpsrv_write(session, data, length);
    }
    
    return(retval);
}
/* EOF */
