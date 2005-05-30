/*****************************************************************************\
 *  $Id: cerebro_metric.c,v 1.4 2005-05-30 05:21:19 achu Exp $
\*****************************************************************************/

#if HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#include <sys/select.h>
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else  /* !TIME_WITH_SYS_TIME */
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else /* !HAVE_SYS_TIME_H */
#  include <time.h>
# endif /* !HAVE_SYS_TIME_H */
#endif /* !TIME_WITH_SYS_TIME */
#include <sys/types.h>
#if HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#include <errno.h>

#include "cerebro.h"
#include "cerebro_api.h"
#include "cerebro_clusterlist_util.h"
#include "cerebro_config_util.h"
#include "cerebro_marshalling.h"
#include "cerebro_module.h"
#include "cerebro_nodelist_util.h"
#include "cerebro_util.h"
#include "cerebro/cerebro_error.h"
#include "cerebro/cerebro_metric_protocol.h"
#include "fd.h"

#if 0
/* 
 * _cerebro_handle_metric_data_check
 *
 * Check for a proper metric_data handle, setting the errnum
 * appropriately if an error is found.
 *
 * Returns 0 on success, -1 on error
 */
static int
_cerebro_handle_metric_data_check(cerebro_t handle, 
				  struct cerebro_metric_data *metric_data)
{
#if CEREBRO_DEBUG
  if (!metric_data)
    {
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }
#endif /* CEREBRO_DEBUG */

  if (metric_data->magic != CEREBRO_METRIC_MAGIC_NUMBER)
    {
      handle->errnum = CEREBRO_ERR_MAGIC_NUMBER;
      return -1;
    }

  return 0;
}

/* 
 * _cerebro_handle_metric_data_loaded_check
 *
 * Checks if the handle contains properly loaded metric_data data.
 * 
 * Returns 0 on success, -1 on error
 */
static int
_cerebro_handle_metric_data_loaded_check(cerebro_t handle)
{
  struct cerebro_metric_data *metric_data;

  if (_cerebro_handle_check(handle) < 0)
    return -1;

  if (!(handle->loaded_state & CEREBRO_METRIC_DATA_LOADED))
    {
      handle->errnum = CEREBRO_ERR_NOT_LOADED;
      return -1;
    }

  metric_data = (struct cerebro_metric_data *)handle->metric_data;
  if (_cerebro_handle_metric_data_check(handle, metric_data) < 0)
    return -1;

  return 0;
}

/* 
 * _cerebro_metric_protocol_err_conversion
 *
 * Convert metric protocol err codes to API err codes
 *
 * Returns proper err code
 */
static int
_cerebro_metric_protocol_err_conversion(u_int32_t protocol_error)
{
  switch(protocol_error)
    {
    case CEREBRO_METRIC_PROTOCOL_ERR_SUCCESS:
      return CEREBRO_ERR_SUCCESS;
    case CEREBRO_METRIC_PROTOCOL_ERR_VERSION_INVALID:
      return CEREBRO_ERR_VERSION_INCOMPATIBLE;
    case CEREBRO_METRIC_PROTOCOL_ERR_PACKET_INVALID:
      return CEREBRO_ERR_PROTOCOL;
    case CEREBRO_METRIC_PROTOCOL_ERR_METRIC_REQUEST_INVALID:
    case CEREBRO_METRIC_PROTOCOL_ERR_TIMEOUT_INVALID:
    case CEREBRO_METRIC_PROTOCOL_ERR_INTERNAL_SYSTEM_ERROR:
    default:
      cerebro_err_debug_lib("%s(%s:%d): invalid protocol error code: %d",
			    __FILE__, __FUNCTION__, __LINE__, 
			    protocol_error);
      return CEREBRO_ERR_INTERNAL;
    }
}

#endif /* 0 */

/* 
 * _cerebro_metric_request_marshall
 *
 * Marshall contents of a metric server request
 *
 * Returns length written to buffer on success, -1 on error
 */
static int
_cerebro_metric_request_marshall(cerebro_t handle,
                                 struct cerebro_metric_request *req,
                                 char *buf,
                                 unsigned int buflen)
{
  int len, count = 0;

#if CEREBRO_DEBUG
  if (!buf)
    {
      cerebro_err_debug_lib("%s(%s:%d): buf null",
			    __FILE__, __FUNCTION__, __LINE__);
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }

  if (buflen < CEREBRO_METRIC_REQUEST_LEN)
    {
      cerebro_err_debug_lib("%s(%s:%d): buflen invalid",
			    __FILE__, __FUNCTION__, __LINE__);
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }

#endif /* CEREBRO_DEBUG */

  memset(buf, '\0', buflen);

  if ((len = _cerebro_marshall_int32(req->version,
                                     buf + count, 
                                     buflen - count)) < 0)
    {
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }
  count += len;

  if ((len = _cerebro_marshall_buffer(req->metric_name,
				      sizeof(req->metric_name),
				      buf + count,
				      buflen - count)) < 0)
    {
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }
  count += len;

  if ((len = _cerebro_marshall_unsigned_int32(req->flags,
                                              buf + count, 
                                              buflen - count)) < 0)
    {
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }
  count += len;

  if ((len = _cerebro_marshall_unsigned_int32(req->timeout_len,
                                              buf + count, 
                                              buflen - count)) < 0)
    {
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }
  count += len;

  return count;
}

#if 0

/* 
 * _cerebro_metric_response_unmarshall
 *
 * Marshall contents of a metric server request
 *
 * Returns 0 on success, -1 on error
 */
static int
_cerebro_metric_response_unmarshall(cerebro_t handle,
                                    struct cerebro_metric_response *res,
                                    const char *buf,
                                    unsigned int buflen)
{
  int len, count = 0;

#if CEREBRO_DEBUG
  if (!buf)
    {
      cerebro_err_debug_lib("%s(%s:%d): buf null",
			    __FILE__, __FUNCTION__, __LINE__);
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }
#endif /* CEREBRO_DEBUG */

  if ((len = _cerebro_unmarshall_int32(&(res->version),
                                       buf + count,
                                       buflen - count)) < 0)
    {
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }
  if (!len)
    return count;

  count += len;

  if ((len = _cerebro_unmarshall_unsigned_int32(&(res->metric_err_code),
                                                buf + count,
                                                buflen - count)) < 0)
    {
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }
  if (!len)
    return count;

  count += len;

  if ((len = _cerebro_unmarshall_unsigned_int8(&(res->end_of_responses),
                                               buf + count,
                                               buflen - count)) < 0)
    {
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }
  if (!len)
    return count;

  count += len;

  if ((len = _cerebro_unmarshall_buffer(res->nodename,
                                        sizeof(res->nodename),
                                        buf + count,
                                        buflen - count)) < 0)
    {
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }
  if (!len)
    return count;

  count += len;

  if ((len = _cerebro_unmarshall_unsigned_int8(&(res->metric_state),
                                               buf + count,
                                               buflen - count)) < 0)
    {
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }
  if (!len)
    return count;

  count += len;

  return count;
}

#endif /* 0 */

/* 
 * _cerebro_metric_request_send
 *
 * Send the metric request
 *
 * Returns 0 on success, -1 on error
 */
static int
_cerebro_metric_request_send(cerebro_t handle,
                             int fd,
			     const char *metric_name,
                             unsigned int timeout_len,
                             int flags)
{
  struct cerebro_metric_request req;
  char buf[CEREBRO_PACKET_BUFLEN];
  int req_len;

  req.version = CEREBRO_METRIC_PROTOCOL_VERSION;
  strncpy(req.metric_name, metric_name, CEREBRO_METRIC_NAME_MAXLEN);
  req.flags = flags;
  req.timeout_len = timeout_len;
  
  if ((req_len = _cerebro_metric_request_marshall(handle,
                                                  &req,
                                                  buf,
                                                  CEREBRO_PACKET_BUFLEN)) < 0)
    return -1;

  if (fd_write_n(fd, buf, req_len) < 0)
    {
      cerebro_err_debug_lib("%s(%s:%d): fd_write_n: %s",
			    __FILE__, __FUNCTION__, __LINE__,
			    strerror(errno));
      handle->errnum = CEREBRO_ERR_INTERNAL;
      return -1;
    }

  return 0;
}

#if 0

/* 
 * _cerebro_metric_response_receive_one
 *
 * Receive a single response
 *
 * Returns response packet and length of packet unmarshalled on
 * success, -1 on error
 */
static int
_cerebro_metric_response_receive_one(cerebro_t handle,
				     int fd,
				     struct cerebro_metric_response *res)
{
  int rv, bytes_read = 0;
  char buf[CEREBRO_PACKET_BUFLEN];

  memset(buf, '\0', CEREBRO_PACKET_BUFLEN);
  while (bytes_read < CEREBRO_METRIC_RESPONSE_LEN)
    {
      fd_set rfds;
      struct timeval tv;
      int num;

      tv.tv_sec = CEREBRO_METRIC_PROTOCOL_CLIENT_TIMEOUT_LEN;
      tv.tv_usec = 0;

      FD_ZERO(&rfds);
      FD_SET(fd, &rfds);

      if ((num = select(fd + 1, &rfds, NULL, NULL, &tv)) < 0)
        {
	  cerebro_err_debug_lib("%s(%s:%d): select: %s",
				__FILE__, __FUNCTION__, __LINE__,
				strerror(errno));
          handle->errnum = CEREBRO_ERR_INTERNAL;
          goto cleanup;
        }
      
      if (!num)
        {
          /* Timed out.  If atleast some bytes were read, unmarshall
           * the received bytes.  Its possible we are expecting more
           * bytes than the client is sending, perhaps because we are
           * using a different protocol version.  This will allow the
           * server to return a invalid version number back to the
           * user.
           */
          if (!bytes_read) 
            {
              handle->errnum = CEREBRO_ERR_PROTOCOL_TIMEOUT;
              goto cleanup;
            }
          else
            goto unmarshall_received;
        }

      if (FD_ISSET(fd, &rfds))
        {
          int n;

          /* Don't use fd_read_n b/c it loops until exactly
           * CEREBRO_METRIC_RESPONSE_LEN is read.  Due to version
           * incompatability or error packets, we may want to read a
           * smaller packet.
           */
          if ((n = read(fd,
                        buf + bytes_read,
                        CEREBRO_METRIC_RESPONSE_LEN - bytes_read)) < 0)
            {
	      cerebro_err_debug_lib("%s(%s:%d): read: %s",
				    __FILE__, __FUNCTION__, __LINE__,
				    strerror(errno));
              handle->errnum = CEREBRO_ERR_INTERNAL;
              goto cleanup;
            }

          if (!n)
            {
              /* Pipe closed */
              handle->errnum = CEREBRO_ERR_PROTOCOL;
              goto cleanup;
            }

          bytes_read += n;
        }
      else
        {
	  cerebro_err_debug_lib("%s(%s:%d): select returned bad data",
				__FILE__, __FUNCTION__, __LINE__);
          handle->errnum = CEREBRO_ERR_INTERNAL;
          goto cleanup;
        }
    }

 unmarshall_received:
  if ((rv = _cerebro_metric_response_unmarshall(handle, 
                                                res, 
                                                buf, 
                                                bytes_read)) < 0)
    goto cleanup;

  return rv;

 cleanup:
  return -1;
}

/* 
 * _cerebro_metric_response_receive_all
 *
 * Receive all of the metric server responses.
 * 
 * Returns 0 on success, -1 on error
 */
static int
_cerebro_metric_response_receive_all(cerebro_t handle,
				     int fd,
				     struct cerebro_metric_data *metric_data,
				     int flags)
{
  struct cerebro_metric_response res;
  int res_len;

  while (1)
    {
      if ((res_len = _cerebro_metric_response_receive_one(handle,
							  fd, 
							  &res)) < 0)
        goto cleanup;

      if (res_len != CEREBRO_METRIC_RESPONSE_LEN)
        {
          if (res_len == CEREBRO_METRIC_RESPONSE_LEN)
            {
              if (res.version != CEREBRO_METRIC_PROTOCOL_VERSION)
                {
                  handle->errnum = CEREBRO_ERR_PROTOCOL;
                  goto cleanup;
                }

              if (res.metric_err_code != CEREBRO_METRIC_PROTOCOL_ERR_SUCCESS)
                {
                  handle->errnum = _cerebro_metric_protocol_err_conversion(res.metric_err_code);
                  goto cleanup;
                }
            }

          handle->errnum = CEREBRO_ERR_PROTOCOL;
          goto cleanup;
        }

      if (res.version != CEREBRO_METRIC_PROTOCOL_VERSION)
        {
          handle->errnum = CEREBRO_ERR_PROTOCOL;
          goto cleanup;
        }
      
      if (res.metric_err_code != CEREBRO_METRIC_PROTOCOL_ERR_SUCCESS)
        {
          handle->errnum = _cerebro_metric_protocol_err_conversion(res.metric_err_code);
          goto cleanup;
        }

      if (res.end_of_responses == CEREBRO_METRIC_PROTOCOL_IS_LAST_RESPONSE)
        break;

      if (flags == CEREBRO_METRIC_UP_NODES
          && res.metric_state != CEREBRO_METRIC_PROTOCOL_STATE_NODE_UP)
        {
          handle->errnum = CEREBRO_ERR_PROTOCOL;
          goto cleanup;
        }
      
      if (flags == CEREBRO_METRIC_DOWN_NODES
          && res.metric_state != CEREBRO_METRIC_PROTOCOL_STATE_NODE_DOWN)
        {
          handle->errnum = CEREBRO_ERR_PROTOCOL;
          goto cleanup;
        }

      if (res.metric_state == CEREBRO_METRIC_PROTOCOL_STATE_NODE_UP)
        {
          if (!metric_data->up_nodes)
            {
	      cerebro_err_debug_lib("%s(%s:%d): up_nodes null",
                                    __FILE__, __FUNCTION__, __LINE__);
              handle->errnum = CEREBRO_ERR_INTERNAL;
              goto cleanup;
            }

          if (!hostlist_push(metric_data->up_nodes, res.nodename))
            {
	      cerebro_err_debug_lib("%s(%s:%d): hostlist_push: %s",
				    __FILE__, __FUNCTION__, __LINE__, 
				    strerror(errno));
              handle->errnum = CEREBRO_ERR_INTERNAL;
              goto cleanup;
            }
        }
      else if (res.metric_state == CEREBRO_METRIC_PROTOCOL_STATE_NODE_DOWN)
        {
          if (!metric_data->down_nodes)
            {
	      cerebro_err_debug_lib("%s(%s:%d): down_nodes null",
                                    __FILE__, __FUNCTION__, __LINE__);
              handle->errnum = CEREBRO_ERR_INTERNAL;
              goto cleanup;
            }

          if (!hostlist_push(metric_data->down_nodes, res.nodename))
            {
	      cerebro_err_debug_lib("%s(%s:%d): hostlist_push: %s",
				    __FILE__, __FUNCTION__, __LINE__, 
				    strerror(errno));
              handle->errnum = CEREBRO_ERR_INTERNAL;
              goto cleanup;
            }
        }
      else
        {
          handle->errnum = CEREBRO_ERR_PROTOCOL;
          goto cleanup;
        }
    }

  if (metric_data->up_nodes)
    hostlist_sort(metric_data->up_nodes);

  if (metric_data->down_nodes)
    hostlist_sort(metric_data->down_nodes);

  return 0;
 cleanup:
  return -1;
}

#endif /* 0 */

/*  
 * _cerebro_metric_get_metric_data
 *
 * Get metric data and store it appropriately into the metric_data
 * structure.
 *
 * Returns 0 on success, -1 on error
 */
static int
_cerebro_metric_get_metric_data(cerebro_t handle,
				cerebro_nodelist_t nodelist,
				const char *metric_name,
				const char *hostname,
				unsigned int port,
				unsigned int timeout_len,
				int flags)
{
  int fd = -1, rv = -1;

  if ((fd = _cerebro_low_timeout_connect(handle, 
					 hostname, 
					 port, 
					 CEREBRO_METRIC_PROTOCOL_CONNECT_TIMEOUT_LEN)) < 0)
    goto cleanup;

  if (_cerebro_metric_request_send(handle, 
				   fd, 
				   metric_name,
				   timeout_len, 
				   flags) < 0)
    goto cleanup;

#if 0
  if (_cerebro_metric_response_receive_all(handle, fd, metric_data, flags) < 0)
    goto cleanup;
#endif /* 0 */
  
  rv = 0;

 cleanup:
  close(fd);
  return rv;
}

cerebro_nodelist_t 
cerebro_get_metric_data(cerebro_t handle, 
			const char *metric_name)
{
  struct cerebro_nodelist *nodelist = NULL;
  unsigned int port;
  unsigned int timeout_len;
  unsigned int flags;

  if (_cerebro_handle_check(handle) < 0)
    goto cleanup;

  if (!metric_name || strlen(metric_name) > CEREBRO_METRIC_NAME_MAXLEN)
    {
      handle->errnum = CEREBRO_ERR_PARAMETERS;
      goto cleanup;
    }

  if (!(handle->loaded_state & CEREBRO_CONFIG_LOADED))
    {
      if (_cerebro_load_config(handle) < 0)
	goto cleanup;
    }

  if (!handle->port)
    {
      if (handle->config_data.cerebro_port_flag)
	{
	  if (!handle->config_data.cerebro_port)
	    {
	      handle->errnum = CEREBRO_ERR_CONFIG_INPUT;
	      goto cleanup;
	    }
	  port = handle->config_data.cerebro_port;
	}
      else
	port = CEREBRO_METRIC_SERVER_PORT;
    }

  if (!timeout_len)
    {
      if (handle->config_data.cerebro_timeout_len_flag)
	{
	  if (!handle->config_data.cerebro_timeout_len)
	    {
	      handle->errnum = CEREBRO_ERR_CONFIG_INPUT;
	      goto cleanup;
	    }
	  timeout_len = handle->config_data.cerebro_timeout_len;
	}
      else
	timeout_len = CEREBRO_METRIC_UPDOWN_TIMEOUT_LEN_DEFAULT;
    }

  if (!flags)
    {
      if (handle->config_data.cerebro_flags_flag)
	{
	  /* XXX should check for valid flags */
#if 0
	  if (handle->config_data.cerebro_flags != CEREBRO_METRIC_UP_NODES
	      && handle->config_data.cerebro_flags != CEREBRO_METRIC_DOWN_NODES
	      && handle->config_data.cerebro_flags != CEREBRO_METRIC_UP_AND_DOWN_NODES)
	    {
	      handle->errnum = CEREBRO_ERR_CONFIG_INPUT;
	      goto cleanup;
	    }
#endif /* 0 */
	  flags = handle->config_data.cerebro_flags;
	}
      else
	flags = 0;		/* XXX maybe different default */
    }
  
  if (!(nodelist = _cerebro_nodelist_create(handle)))
    goto cleanup;

  if (!strlen(handle->hostname))
    {
      if (handle->config_data.cerebro_hostnames_flag)
	{
	  int i, rv = -1;

	  for (i = 0; i < handle->config_data.cerebro_hostnames_len; i++)
	    {
	      if ((rv = _cerebro_metric_get_metric_data(handle,
							nodelist,
							metric_name,
							handle->config_data.cerebro_hostnames[i],
							port,
							timeout_len,
							flags)) < 0)
		continue;
	      break;
	    }
	  
          if (i >= handle->config_data.cerebro_hostnames_len)
            {
              handle->errnum = CEREBRO_ERR_CONNECT;
              goto cleanup;
            }

	  if (rv < 0)
            goto cleanup;
	}
      else
	{
	  if (_cerebro_metric_get_metric_data(handle,
					      nodelist,
					      metric_name,
					      "localhost",
					      port,
					      timeout_len,
					      flags) < 0)
	    goto cleanup;
	}
    }
  else
    {
      if (_cerebro_metric_get_metric_data(handle,
					  nodelist,
					  metric_name,
					  handle->hostname,
					  port,
					  timeout_len,
					  flags) < 0)
	goto cleanup;
    }

  handle->errnum = CEREBRO_ERR_SUCCESS;
  return nodelist;
  
 cleanup:
  if (nodelist)
    (void)cerebro_nodelist_destroy(nodelist);
  return NULL;
}