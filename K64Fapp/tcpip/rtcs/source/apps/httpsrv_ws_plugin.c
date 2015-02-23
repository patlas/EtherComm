/*HEADER**********************************************************************
*
* Copyright 2014 Freescale Semiconductor, Inc.
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
*   This file contains implementation of WebSocket protocol plugin for HTTPSRV.
*
*END************************************************************************/

#include <httpsrv_ws_prv.h>
#include <httpsrv_prv.h>
#include <lwmsgq.h>

static int ws_plugin_insert_sock(WS_PLUGIN_CONTEXT *context, uint32_t sock);
static void ws_plugin_remove_invalid(WS_PLUGIN_CONTEXT *context);
static void ws_plugin_stop(WS_PLUGIN_CONTEXT *context);

/*
 * Create context for WebSocket plugin.
 */
void *ws_plugin_create_context(void *message)
{
    void *retval;

    retval = _mem_alloc_zero(sizeof(WS_PLUGIN_CONTEXT));

    if (retval != NULL)
    {
        RTCS_FD_ZERO(&(((WS_PLUGIN_CONTEXT *) retval)->rfds));
    }
    
    return(retval);
}

/*
 * Destroy WebSocket plugin context.
 */
void ws_plugin_destroy_context(void *plugin_context)
{
    uint32_t           n = 0;
    bool               wait = FALSE;
    WS_PLUGIN_CONTEXT  *context;
    
    if (plugin_context == NULL)
    {
        return;
    }

    context = (WS_PLUGIN_CONTEXT *) plugin_context;
    
    /* Wait until all WebSocket tasks end. */
    do
    {
        wait = FALSE;
        for (n = 0; n < RTCSCFG_FD_SETSIZE; n++)
        {
            if (context->tids[n] != 0)
            {
                wait = TRUE;
                break;
            }
        }
        _sched_yield();
    }while(wait);

    _mem_free(plugin_context);
}

/*
 * Process WebScoket plugin message.
 */
void ws_plugin_process(void *msg, void *plugin_context)
{
    WS_PLUGIN_CONTEXT  *context;
    int                index;
    HTTPSRV_PLUGIN_MSG *message;

    message = (HTTPSRV_PLUGIN_MSG *) msg;
    context = (WS_PLUGIN_CONTEXT *) plugin_context;

    /* Stop the plugin tasks if necessary */
    if (message->plugin == NULL)
    {
        ws_plugin_stop(context);
        return;
    }
    
    /* First remove sockets which no longer has running task. */
    ws_plugin_remove_invalid(context);

    /* Insert new socket to socket arrays. */
    RTCS_FD_SET(message->sock, &context->rfds);
    index = ws_plugin_insert_sock(context, message->sock);
    if (index < 0)
    {
        return;
    }
    context->sock_count++;

    /* If there is no task to service socket, create it. */
    if (context->tids[index] == 0)
    {
        WS_INIT_STRUCT ws_init = {0};

        ws_init.socket = message->sock;
        ws_init.plugin = (WS_PLUGIN_STRUCT *) message->plugin->data;
        ws_init.buffer = message->buffer;
        ws_init.buf_len = HTTPSRV_SES_BUF_SIZE_PRV;
        ws_init.tid = context->tids+index;
        ws_init.msg_queue = context->queues+index;

        /* Run WebSocket session task. */
        if (RTCS_task_create(WS_SESSION_TASK_NAME, 8/*server->params.server_prio*/, HTTPSRV_SESSION_STACK_SIZE, ws_session_task, &ws_init) != RTCS_OK)
        {
            return;
        }
        /* Transfer buffer to WebSocket task. */
        _mem_transfer(ws_init.buffer, _task_get_id(), *ws_init.tid);
    }
}

/*
 * Stop all WebSocket tasks.
 */
static void ws_plugin_stop(WS_PLUGIN_CONTEXT *context)
{
    WS_API_CALL_MSG   message;
    uint32_t          i;
    
    message.command = WS_COMMAND_CLOSE;

    /* Find sockets with available data. */
    for (i = 0; i < RTCSCFG_FD_SETSIZE; i++)
    {
        if (context->queues[i] != NULL)
        {
            _lwmsgq_send((void *) context->queues[i], (_mqx_max_type *) &message, LWMSGQ_SEND_BLOCK_ON_FULL);
        }
    }
}

/*
 * Remove all sockets with ended tasks.
 */
static void ws_plugin_remove_invalid(WS_PLUGIN_CONTEXT *context)
{
    uint32_t i;

    for (i = 0; i < RTCSCFG_FD_SETSIZE; i++)
    {
        if ((context->socks[i] != 0) && (context->tids[i] == 0))
        {
            RTCS_FD_CLR(context->socks[i], &context->rfds);
            context->socks[i] = 0;
        }
    }
}

/*
 * Insert socket to plugin processing.
 */
static int ws_plugin_insert_sock(WS_PLUGIN_CONTEXT *context, uint32_t sock)
{
    uint32_t i;
    int      i_free;
    int      i_found;
    int      retval;
    
    i_free = -1;
    i_found = -1;

    for (i = 0; i < RTCSCFG_FD_SETSIZE; i++)
    {
        if ((context->socks[i] == 0) && (i_free == -1)) 
        {
            i_free = i;
        }
        else if (context->socks[i] == sock)
        {
            i_found = i;
            break;
        }
    }

    if (i_found > -1)
    {
        retval = i_found;
    }
    else
    {
        retval = i_free;
        if (i_free > -1)
        {
            context->socks[i_free] = sock;
        }
    }
    return(retval);
}

/*
 * Wait for activity on all WebSocket sockets.
 */
void ws_plugin_wait(void *plugin_context)
{
    WS_PLUGIN_CONTEXT *context;
    rtcs_fd_set       rfds;
    uint32_t          i;
    uint32_t          sock;
    WS_API_CALL_MSG   message;
    uint32_t          n_valid;

    context = (WS_PLUGIN_CONTEXT *) plugin_context;
    rfds = context->rfds;
    message.command = WS_COMMAND_RECV;
    
    /* First remove sockets which no longer has running task. */
    ws_plugin_remove_invalid(context);
    
    n_valid = select(context->sock_count, &rfds, 0, WS_PLUGIN_WAIT_ACTIVITY);

    if ((n_valid == RTCS_ERROR) || (n_valid == 0))
    {
        return;
    }
    
    /* Find sockets with available data. */
    for (i = 0; i < RTCSCFG_FD_SETSIZE; i++)
    {
        sock = context->socks[i];
        if (RTCS_FD_ISSET(sock, &rfds))
        {
            uint32_t error;
            /* 
             * Remove socket from select queue. It will be added back when 
             * receive in complete in WebSocket task.
             */
            RTCS_FD_CLR(sock, &context->rfds);
            message.data = (void *) &context->rfds;
            error = _lwmsgq_send((void *) context->queues[i], (_mqx_max_type *) &message, LWMSGQ_SEND_BLOCK_ON_FULL);
            if (error != MQX_OK)
            {
                break;
            }            
        }
    }
}

/*
 * Allocate WebSocket handshake structure.
 */
void* httpsrv_ws_alloc(HTTPSRV_SESSION_STRUCT *session)
{
    if (session->ws_handshake == NULL)
    {
        session->ws_handshake = _mem_alloc_zero(sizeof(WS_HANDSHAKE_STRUCT));
    }
    if (session->ws_handshake == NULL)
    {
        session->state = HTTPSRV_SES_RESP;
        session->response.status_code = HTTPSRV_CODE_INTERNAL_ERROR;
    }
    return(session->ws_handshake);
}
