#ifndef __rtcs_ssl_h__
#define __rtcs_ssl_h__

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
*   SSL adaptation layer for RTCS
*
*
*END************************************************************************/

#include <stdint.h>

#if RTCSCFG_ENABLE_SSL == 1

#include <cyassl/ssl.h>

#define RTCS_ssl_recv(ctx, dest, len) CyaSSL_read((CYASSL *) ctx, dest, len)
#define RTCS_ssl_send(ctx, dest, len) CyaSSL_write((CYASSL *) ctx, dest, len)

#else

uint32_t RTCS_ssl_recv(uint32_t ctx, char *dest, uint32_t len);
uint32_t RTCS_ssl_send(uint32_t ctx, char *dest, uint32_t len);

#endif /* RTCSCFG_ENABLE_SSL */

typedef enum rtcs_ssl_init_type
{
    RTCS_SSL_SERVER,
    RTCS_SSL_CLIENT
}RTCS_SSL_INIT_TYPE;

typedef struct rtcs_ssl_params_struct
{
    char*              cert_path;
    char*              priv_key_path;
    char*              verify_path;
    RTCS_SSL_INIT_TYPE init_type;
}RTCS_SSL_PARAMS_STRUCT;

uint32_t RTCS_ssl_socket(void* ctx, uint32_t sock);
void RTCS_ssl_shutdown(uint32_t sock);
void* RTCS_ssl_init(RTCS_SSL_PARAMS_STRUCT *params);

#endif // __rtcs_ssl_h__
