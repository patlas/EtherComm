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

#include <rtcs.h>
#include <rtcs_ssl.h>
#include <rtcs_err.h>

/* Create SSL connection using sock, return SSL connection handle. */
uint32_t RTCS_ssl_socket(void* ctx, uint32_t sock)
{
    uint32_t   retval;

    retval = RTCS_SOCKET_ERROR;

    #if RTCSCFG_ENABLE_SSL == 1
    CYASSL_CTX *ssl_ctx;
    CYASSL     *ssl_sock;
    
    ssl_ctx = (CYASSL_CTX *) ctx;
    
    if ((ssl_sock = CyaSSL_new(ssl_ctx)) != NULL)
    {
        if (CyaSSL_set_fd(ssl_sock, sock) == SSL_SUCCESS)
        {
            retval = (uint32_t) ssl_sock;
        }
        else
        {
            CyaSSL_free(ssl_sock);
        }
    }
    #endif

    return(retval);
}

/* Shutdown SSL connection. */
void RTCS_ssl_shutdown(uint32_t sock)
{
    #if RTCSCFG_ENABLE_SSL == 1
    CYASSL     *ssl_sock;

    ssl_sock = (CYASSL *) sock;
    CyaSSL_shutdown(ssl_sock);
    CyaSSL_free(ssl_sock);
    #endif
}

#if RTCSCFG_ENABLE_SSL == 0
/* Receive data from SSL layer - return error, SSL is disabled. */
uint32_t RTCS_ssl_recv(uint32_t ctx, char *dest, uint32_t len)
{
    return(RTCS_ERROR);
}

/* Send data through SSL layer - return error, SSL is disabled. */
uint32_t RTCS_ssl_send(uint32_t ctx, char *dest, uint32_t len)
{
    return(RTCS_ERROR);
}

#endif

/* Initialize CyaSSL */
void* RTCS_ssl_init(RTCS_SSL_PARAMS_STRUCT *params)
{
    void* retval;

    retval = NULL;


    #if RTCSCFG_ENABLE_SSL == 1
    CYASSL_CTX *ctx;

    CyaSSL_Init();

    switch (params->init_type)
    {
        case RTCS_SSL_SERVER:
            ctx = CyaSSL_CTX_new(CyaSSLv23_server_method());
            if (ctx == NULL)
            {
                goto END;
            }
            if (CyaSSL_CTX_use_certificate_file(ctx, params->cert_path, SSL_FILETYPE_PEM) != SSL_SUCCESS)
            {
                goto END;
            }
            if (CyaSSL_CTX_use_PrivateKey_file(ctx, params->priv_key_path, SSL_FILETYPE_PEM) != SSL_SUCCESS)
            {
                goto END;
            }
            break;
        case RTCS_SSL_CLIENT:
            ctx = CyaSSL_CTX_new(CyaSSLv23_client_method());
            if (CyaSSL_CTX_load_verify_locations(ctx, params->verify_path, 0) != SSL_SUCCESS)
            {
                goto END;
            }
            break;
        default:
            goto END;
            break;
    }

    retval = (void *) ctx;
    END:
    #endif
    return(retval);
}