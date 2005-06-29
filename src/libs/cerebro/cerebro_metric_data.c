/*****************************************************************************\
 *  $id: cerebro_metric.c,v 1.17 2005/06/07 16:18:58 achu Exp $
\*****************************************************************************/

#if HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */

#include "cerebro.h"
#include "cerebro_api.h"
#include "cerebro_clusterlist_util.h"
#include "cerebro_nodelist_util.h"
#include "cerebro_util.h"
#include "cerebro/cerebro_metric_protocol.h"

#include "cerebro_metric_util.h"

#include "debug.h"
#include "fd.h"
#include "marshall.h"
#include "network_util.h"

/* 
 * _cerebro_node_metric_response_header_unmarshall
 *
 * Unmarshall contents of a metric server response header
 *
 * Returns 0 on success, -1 on error
 */
static int
_cerebro_node_metric_response_header_unmarshall(cerebro_t handle,
                                                struct cerebro_node_metric_response *res,
                                                const char *buf,
                                                unsigned int buflen)
{
  int n, len = 0;

  if (!res || !buf)
    {
      CEREBRO_DBG(("invalid pointers"));
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }

  if ((n = unmarshall_int32(&(res->version), buf + len, buflen - len)) < 0)
    {
      CEREBRO_DBG(("unmarshall_int32"));
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }

  if (!n)
    return len;
  len += n;

  if ((n = unmarshall_u_int32(&(res->err_code), buf + len, buflen - len)) < 0)
    {
      CEREBRO_DBG(("unmarshall_u_int32"));
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }

  if (!n)
    return len;
  len += n;

  if ((n = unmarshall_u_int8(&(res->end), buf + len, buflen - len)) < 0)
    {
      CEREBRO_DBG(("unmarshall_u_int8"));
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }

  if (!n)
    return len;
  len += n;

  if ((n = unmarshall_buffer(res->nodename,
                             sizeof(res->nodename),
                             buf + len,
                             buflen - len)) < 0)
    {
      CEREBRO_DBG(("unmarshall_buffer"));
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }

  if (!n)
    return len;
  len += n;

  if ((n = unmarshall_u_int32(&(res->metric_value_type), 
                              buf + len, 
                              buflen - len)) < 0)
    {
      CEREBRO_DBG(("unmarshall_u_int32"));
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }

  if (!n)
    return len;
  len += n;
  
  if ((n = unmarshall_u_int32(&(res->metric_value_len),
                              buf + len,
                              buflen - len)) < 0)
    {
      CEREBRO_DBG(("unmarshall_u_int32"));
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }

  if (!n)
    return len;
  len += n;

  return len;
}

/* 
 * _cerebro_node_metric_response_metric_value_unmarshall
 *
 * Unmarshall contents of a metric server response
 *
 * Returns 0 on success, -1 on error
 */
static int
_cerebro_node_metric_response_metric_value_unmarshall(cerebro_t handle,
                                                      struct cerebro_node_metric_response *res,
                                                      const char *buf,
                                                      unsigned int buflen)
{
  int malloc_len = 0, err_flag = 0;

#if CEREBRO_DEBUG
  if (!res || !buf)
    {
      CEREBRO_DBG(("invalid pointers"));
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }
#endif /* CEREBRO_DEBUG */

  switch(res->metric_value_type)
    {
    case CEREBRO_METRIC_VALUE_TYPE_NONE:
      cerebro_err_debug("%s(%s:%d): type none despite data returned",
			__FILE__, __FUNCTION__, __LINE__);
      err_flag++;
      break;
    case CEREBRO_METRIC_VALUE_TYPE_INT32:
      if (buflen != sizeof(int32_t))
        err_flag++;
      break;
    case CEREBRO_METRIC_VALUE_TYPE_U_INT32:
      if (buflen != sizeof(u_int32_t))
        err_flag++;
      break;
    case CEREBRO_METRIC_VALUE_TYPE_FLOAT:
      if (buflen != sizeof(float))
        err_flag++;
      break;
    case CEREBRO_METRIC_VALUE_TYPE_DOUBLE:
      if (buflen != sizeof(double))
        err_flag++;
      break;
    case CEREBRO_METRIC_VALUE_TYPE_STRING:
      break;
    default:
      err_flag++;
    };

  if (err_flag)
    {
      handle->errnum = CEREBRO_ERR_PROTOCOL;
      return -1;
    }

  if (res->metric_value_type == CEREBRO_METRIC_VALUE_TYPE_STRING)
    malloc_len = buflen;
  else
    malloc_len = buflen + 1;

  if (!(res->metric_value = malloc(buflen)))
    {
      handle->errnum = CEREBRO_ERR_OUTMEM;
      return -1;
    }
  memset(res->metric_value, '\0', malloc_len);

  err_flag = 0;
  switch(res->metric_value_type)
    {
    case CEREBRO_METRIC_VALUE_TYPE_INT32:
      if (unmarshall_int32((int32_t *)res->metric_value, buf, buflen) < 0)
        {
          CEREBRO_DBG(("unmarshall_int32"));
          err_flag++;
        }
      break;
    case CEREBRO_METRIC_VALUE_TYPE_U_INT32:
      if (unmarshall_u_int32((u_int32_t *)res->metric_value, buf, buflen) < 0)
        {
          CEREBRO_DBG(("unmarshall_u_int32"));
          err_flag++;
        }
      break;
    case CEREBRO_METRIC_VALUE_TYPE_FLOAT:
      if (unmarshall_float((float *)res->metric_value, buf, buflen) < 0)
        {
          CEREBRO_DBG(("unmarshall_float"));
          err_flag++;
        }
      break;
    case CEREBRO_METRIC_VALUE_TYPE_DOUBLE:
      if (unmarshall_double((double *)res->metric_value, buf, buflen) < 0)
        {
          CEREBRO_DBG(("unmarshall_double"));
          err_flag++;
        }
      break;
    case CEREBRO_METRIC_VALUE_TYPE_STRING:
      if (unmarshall_buffer((char *)res->metric_value,
                            res->metric_value_len,
                            buf,
                            buflen) < 0)
        {
          CEREBRO_DBG(("unmarshall_buffer"));
          err_flag++;
        }
      break;
    default:
      err_flag++;
    };

  if (err_flag)
    {
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }

  return buflen;
}

/* 
 * _receive_node_metric_responses
 *
 * Receive all of the metric server responses.
 * 
 * Returns 0 on success, -1 on error
 */
static int
_receive_node_metric_responses(cerebro_t handle, void *list, int fd)
{
  struct cerebro_nodelist *nodelist;

  if (_cerebro_handle_check(handle) < 0)
    {
      CEREBRO_DBG(("handle invalid"));
      handle->errnum = CEREBRO_ERR_INTERNAL;
      goto cleanup;
    }

  if (!list || fd <= 0)
    {
      CEREBRO_DBG(("invalid parameters"));
      handle->errnum = CEREBRO_ERR_INTERNAL;
      goto cleanup;
    }

  nodelist = (struct cerebro_nodelist *)list;
  if (nodelist->magic != CEREBRO_NODELIST_MAGIC_NUMBER)
    {
      CEREBRO_DBG(("invalid parameters"));
      handle->errnum = CEREBRO_ERR_INTERNAL;
      goto cleanup;
    }

  /* XXX fix cleanup, not quite right */
  while (1)
    {
      struct cerebro_node_metric_response res;
      char buf[CEREBRO_MAX_PACKET_LEN];
      char nodename_buf[CEREBRO_MAX_NODENAME_LEN+1];
      int bytes_read, header_len;
      unsigned int errnum;

      if ((bytes_read = receive_data(fd,
                                     CEREBRO_NODE_METRIC_RESPONSE_HEADER_LEN,
                                     buf,
                                     CEREBRO_MAX_PACKET_LEN,
                                     CEREBRO_METRIC_PROTOCOL_CLIENT_TIMEOUT_LEN,
                                     &errnum)) < 0)
        {
          handle->errnum = errnum;
          goto cleanup;
        }

      if (bytes_read < CEREBRO_METRIC_ERR_RESPONSE_LEN)
        {
          handle->errnum = CEREBRO_ERR_PROTOCOL;
          goto cleanup;
        }

      if (_cerebro_metric_response_check(handle, buf, bytes_read) < 0)
        goto cleanup;

      if ((header_len = _cerebro_node_metric_response_header_unmarshall(handle, 
                                                                        &res, 
                                                                        buf, 
                                                                        bytes_read)) < 0)
        goto cleanup;
      
      if (header_len < CEREBRO_NODE_METRIC_RESPONSE_HEADER_LEN)
        {
          handle->errnum = CEREBRO_ERR_PROTOCOL;
          goto cleanup;
        }
      
      if (res.metric_value_len)
        {
          char *value = NULL;
          int value_read, value_count;
          unsigned int errnum;

          if (res.metric_value_type == CEREBRO_METRIC_VALUE_TYPE_NONE)
            {
              handle->errnum = CEREBRO_ERR_PROTOCOL;
              goto cleanup;
            }

          if (!(value = malloc(res.metric_value_len)))
            {
              handle->errnum = CEREBRO_ERR_OUTMEM;
              goto cleanup;
            }

          if ((value_read = receive_data(fd,
                                         res.metric_value_len,
                                         value,
                                         res.metric_value_len,
                                         CEREBRO_METRIC_PROTOCOL_CLIENT_TIMEOUT_LEN,
                                         &errnum)) < 0)
            {
              handle->errnum = errnum;
              free(value);
              goto cleanup;
            }
      
          if (value_read < res.metric_value_len)
            {
              handle->errnum = CEREBRO_ERR_PROTOCOL;
              free(value);
              goto cleanup;
            }

          if ((value_count = _cerebro_node_metric_response_metric_value_unmarshall(handle,
                                                                                   &res,
                                                                                   value,
                                                                                   value_read)) < 0)
            {
              free(value);
              goto cleanup;
            }

          if (value_count < res.metric_value_len)
            {
              handle->errnum = CEREBRO_ERR_PROTOCOL;
              free(value);
              goto cleanup;
            }

          res.metric_value = value;
        }
      else
        res.metric_value = NULL;

      if (res.end == CEREBRO_METRIC_PROTOCOL_IS_LAST_RESPONSE)
        {
          if (res.metric_value)
            {
              handle->errnum = CEREBRO_ERR_PROTOCOL;
              free(res.metric_value);
              goto cleanup;
            }
          break;
        }

      /* Guarantee ending '\0' character */
      memset(nodename_buf, '\0', CEREBRO_MAX_NODENAME_LEN+1);
      memcpy(nodename_buf, res.nodename, CEREBRO_MAX_NODENAME_LEN);

      if (_cerebro_nodelist_append(nodelist, 
                                   nodename_buf,
                                   res.metric_value_type,
                                   res.metric_value_len,
                                   res.metric_value) < 0)
        {
          if (res.metric_value)
            free(res.metric_value);
          goto cleanup;
        }

      if (res.metric_value)
        free(res.metric_value);
    }
  

  if (_cerebro_nodelist_sort(nodelist) < 0)
    goto cleanup;

  return 0;

 cleanup:
  return -1;
}

cerebro_nodelist_t 
cerebro_get_metric_data(cerebro_t handle, const char *metric_name)
{
  struct cerebro_nodelist *nodelist = NULL;

  if (_cerebro_handle_check(handle) < 0)
    goto cleanup;

  if (!metric_name || strlen(metric_name) > CEREBRO_MAX_METRIC_NAME_LEN)
    {
      handle->errnum = CEREBRO_ERR_PARAMETERS;
      goto cleanup;
    }

  if (_cerebro_load_clusterlist_module(handle) < 0)
    goto cleanup;
  
  if (!(nodelist = _cerebro_nodelist_create(handle, metric_name)))
    goto cleanup;

  if (_cerebro_metric_connect_and_receive(handle,
                                          nodelist,
                                          metric_name,
                                          _receive_node_metric_responses) < 0)
    goto cleanup;
  
  handle->errnum = CEREBRO_ERR_SUCCESS;
  return nodelist;
  
 cleanup:
  if (nodelist)
    (void)cerebro_nodelist_destroy(nodelist);
  return NULL;
}
