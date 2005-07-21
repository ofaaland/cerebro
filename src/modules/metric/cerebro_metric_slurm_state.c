/*****************************************************************************\
 *  $Id: cerebro_metric_slurm_state.c,v 1.12 2005-07-21 23:11:26 achu Exp $
\*****************************************************************************/

#if HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/un.h>
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
#if HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#if HAVE_PTHREAD_H
#include <pthread.h>
#endif /* HAVE_PTHREAD_H */

#include "cerebro.h"
#include "cerebro/cerebro_constants.h"
#include "cerebro/cerebro_metric_module.h"

#include "debug.h"

#define SLURM_STATE_METRIC_MODULE_NAME      "slurm_state"
#define SLURM_STATE_METRIC_NAME             "slurm_state"
#define SLURM_STATE_BACKLOG                 5
#define SLURM_STATE_REINITIALIZE_WAIT_TIME  5

/*
 * metric_slurm_state
 *
 * cached system slurm_state
 */
static u_int32_t metric_slurm_state = 0;

/* 
 * slurm_state_fd
 * 
 * Unix Domain socket in which slurm will connect to
 */
static int slurm_state_fd = -1;

/* 
 * _slurm_state_setup_socket
 *
 * Create and setup the server socket
 *
 * Returns file descriptor on success, -1 on error
 */
static int
_slurm_state_setup_socket(void)
{
  struct sockaddr_un addr;
  int fd;

  if ((fd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0)
    {      
      CEREBRO_DBG(("socket: %s", strerror(errno)));
      goto cleanup;
    }
  
  if (strlen(SLURM_STATE_CONTROL_PATH) >= sizeof(addr.sun_path))
    {
      CEREBRO_DBG(("path '%s' too long", SLURM_STATE_CONTROL_PATH));
      goto cleanup;
    }

  /* unlink is allowed to fail */ 
  unlink(SLURM_STATE_CONTROL_PATH);

  memset(&addr, '\0', sizeof(struct sockaddr_un));
  addr.sun_family = AF_LOCAL;
  strncpy(addr.sun_path, SLURM_STATE_CONTROL_PATH, sizeof(addr.sun_path));

  if (bind(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) < 0)
    {
      CEREBRO_DBG(("bind: %s", strerror(errno)));
      goto cleanup;
    }
  
  if (listen(fd, SLURM_STATE_BACKLOG) < 0)
    {
      CEREBRO_DBG(("listen: %s", strerror(errno)));
      goto cleanup;
    }
  
  return fd;

 cleanup:
  close(fd);
  return -1;
}

/*
 * slurm_state_metric_setup
 *
 * slurm_state metric module setup function.
 */
static int
slurm_state_metric_setup(void)
{
  if ((slurm_state_fd = _slurm_state_setup_socket()) < 0)
    return -1;

  return 0;
}

/*
 * slurm_state_metric_cleanup
 *
 * slurm_state metric module cleanup function
 */
static int
slurm_state_metric_cleanup(void)
{
  /* nothing to do */
  return 0;
}

/*
 * slurm_state_metric_get_metric_name
 *
 * slurm_state metric module get_metric_name function
 */
static char *
slurm_state_metric_get_metric_name(void)
{
  return SLURM_STATE_METRIC_NAME;
}

/*
 * slurm_state_metric_get_metric_period
 *
 * slurm_state metric module get_metric_period function
 */
static int
slurm_state_metric_get_metric_period(int *period)
{
  if (!period)
    {
      CEREBRO_DBG(("invalid parameters"));
      return -1;
    }

  /* The slurm_state is propogated every once in awhile.  The
   * slurm_state metric thread is more responsible for propogation..
   */
  *period = 5*60;
  return 0;
}

/*
 * slurm_state_metric_get_metric_value
 *
 * slurm_state metric module get_metric_value function
 */
static int
slurm_state_metric_get_metric_value(unsigned int *metric_value_type,
                                    unsigned int *metric_value_len,
                                    void **metric_value)
{
  if (!metric_value_type || !metric_value_len || !metric_value)
    {
      CEREBRO_DBG(("invalid parameters"));
      return -1;
    }

  *metric_value_type = CEREBRO_METRIC_VALUE_TYPE_U_INT32;
  *metric_value_len = sizeof(u_int32_t);
  *metric_value = (void *)&metric_slurm_state;

  return 0;
}

/*
 * slurm_state_metric_destroy_metric_value
 *
 * slurm_state metric module destroy_metric_value function
 */
static int
slurm_state_metric_destroy_metric_value(void *metric_value)
{
  return 0;
}

/* 
 * slurm_state_metric_thread
 *
 * Thread that will continually monitor the state of slurm
 */
static void *
slurm_state_metric_thread(void *arg)
{
  Cerebro_metric_updated updated_func;

  if (!arg)
    {
      CEREBRO_DBG(("invalid parameters"));
      return NULL;
    }

  updated_func = (Cerebro_metric_updated)arg;

  while (1)
    {
      struct sockaddr_un addr;
      socklen_t addrlen;
      int fd, num;
      
      addrlen = sizeof(sizeof(struct sockaddr_un));
      if ((fd = accept(slurm_state_fd, (struct sockaddr *)&addr, &addrlen)) < 0)
        {
          CEREBRO_DBG(("accept: %s", strerror(errno)));
          if (errno == EINTR)
            continue;
          else
            {
              /* Make an attempt to set things up again */
              slurm_state_fd = _slurm_state_setup_socket();
              sleep(SLURM_STATE_REINITIALIZE_WAIT_TIME);
            }
        }

      metric_slurm_state = 1;

      /* No biggie if it fails, we can continue */
      if (updated_func(SLURM_STATE_METRIC_MODULE_NAME) < 0)
        CEREBRO_DBG(("updated_func failed"));
      
      while (1)
        {
          char buf[CEREBRO_MAX_PACKET_LEN];
          fd_set rfds;

          FD_ZERO(&rfds);
          FD_SET(fd, &rfds);
          
          if ((num = select(fd + 1, &rfds, NULL, NULL, NULL)) < 0)
            {
              CEREBRO_DBG(("select: %s", strerror(errno)));
              if (errno == EINTR)
                continue;
              else
                {
                  /* Break and we'll try again */
                  sleep(SLURM_STATE_REINITIALIZE_WAIT_TIME);
                  close(fd);
                  break;
                }
            }

          if (!num)
            {
              /* Can this even happen? Re-setup and restart the loop */
              CEREBRO_DBG(("select invalid return"));
              slurm_state_fd = _slurm_state_setup_socket();
              sleep(SLURM_STATE_REINITIALIZE_WAIT_TIME);
              close(fd);
              break;
            }

          if (FD_ISSET(fd, &rfds))
            {
              int n;
              
              if ((n = read(fd, buf, CEREBRO_MAX_PACKET_LEN)) < 0)
                {
                  CEREBRO_DBG(("read: %s", strerror(errno)));
                  sleep(SLURM_STATE_REINITIALIZE_WAIT_TIME);
                  close(fd);
                  break;
                }

              /* Should be 0, but perhaps something was accidently
               * sent.  We'll assume the connection died if partial
               * data was read.
               */
              if (n > 0 && n < CEREBRO_MAX_PACKET_LEN)
                {
                  CEREBRO_DBG(("unintended read: %d", n));
                  n = 0;
                }

              if (!n)
                {
                  metric_slurm_state = 0;
                  
                  /* No biggie if it fails, we can continue */
                  if (updated_func(SLURM_STATE_METRIC_MODULE_NAME) < 0)
                    CEREBRO_DBG(("updated_func failed"));

                  close(fd);
                  break;
                }
            }
          else
            {
              /* Can this even happen? Re-setup and restart the loop */
              CEREBRO_DBG(("select invalid return"));
              slurm_state_fd = _slurm_state_setup_socket();
              sleep(SLURM_STATE_REINITIALIZE_WAIT_TIME);
              close(fd);
              break;
            }
          
        }
    }

  /* NOT REACHED */
  return NULL;
}

/*
 * slurm_state_metric_get_metric_thread
 *
 * slurm_state metric module get_metric_thread function
 */
static Cerebro_metric_thread_pointer
slurm_state_metric_get_metric_thread(void)
{
  return &slurm_state_metric_thread;
}

struct cerebro_metric_module_info metric_module_info =
  {
    SLURM_STATE_METRIC_MODULE_NAME,
    &slurm_state_metric_setup,
    &slurm_state_metric_cleanup,
    &slurm_state_metric_get_metric_name,
    &slurm_state_metric_get_metric_period,
    &slurm_state_metric_get_metric_value,
    &slurm_state_metric_destroy_metric_value,
    &slurm_state_metric_get_metric_thread,
  };
