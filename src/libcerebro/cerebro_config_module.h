/*****************************************************************************\
 *  $Id: cerebro_config_module.h,v 1.3 2005-04-22 18:38:02 achu Exp $
\*****************************************************************************/

#ifndef _CEREBRO_CONFIG_MODULE_H
#define _CEREBRO_CONFIG_MODULE_H

/*  
 * struct cerebrod_module_config
 *
 * passed to config module to load alternate default values
 */
struct cerebrod_module_config
{
  unsigned int heartbeat_frequency_min;
  unsigned int heartbeat_frequency_max;
  int heartbeat_source_port;
  int heartbeat_destination_port;
  char *heartbeat_destination_ip;
  char *heartbeat_network_interface;
  int heartbeat_ttl;
  int speak;
  int listen;
  int listen_threads;
  int updown_server;
  int updown_server_port;
  char *clusterlist_module;
  char **clusterlist_module_options;
};

/*
 * Cerebro_config_load_cerebrod_default
 *
 * function prototype for config module function to alter default
 * configuration values for the cerebrod daemon
 *
 * Returns 0 on success, -1 on error
 */
typedef int (*Cerebro_config_load_cerebrod_default)(struct cerebrod_module_config *conf);

/*
 * struct cerebro_config_module_info 
 * 
 * contains config module information and operations.  Required to be
 * defined in each config module.
 */
struct cerebro_config_module_info
{
  char *config_module_name;
  Cerebro_config_load_cerebrod_default load_cerebrod_default;
};

#endif /* _CEREBRO_CONFIG_MODULE_H */