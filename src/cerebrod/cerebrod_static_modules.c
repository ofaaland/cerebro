/*****************************************************************************\
 *  $Id: cerebrod_static_modules.c,v 1.2 2005-03-29 21:30:29 achu Exp $
\*****************************************************************************/

#if HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#if WITH_STATIC_MODULES

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#include <errno.h>
#include <assert.h>

#include "cerebrod_static_modules.h"
#include "cerebrod_config.h"
#include "cerebrod_clusterlist.h"
#include "cerebrod_error.h"
#include "error.h"

/*
 * static_config_modules
 * static_config_modules_names
 *
 * configuration modules statically compiled in
 */
struct cerebrod_config_module_info *static_config_modules[] =
  {
#if WITH_GENDERSLLNL
    &gendersllnl_config_module_info,
#endif
    NULL
  };

/*
 * static_clusterlist_modules
 *
 * clusterlist modules statically compiled in
 */
struct cerebrod_clusterlist_module_info *static_clusterlist_modules[] =
  {
#if WITH_GENDERSLLNL
    &gendersllnl_clusterlist_module_info,
#endif
#if WITH_GENDERS
    &genders_clusterlist_module_info,
#endif
    &hostsfile_clusterlist_module_info,
    &none_clusterlist_module_info,
    NULL
  };

struct cerebrod_config_module_info *
cerebrod_find_static_config_module(char *name)
{
  struct cerebrod_config_module_info **ptr = &static_config_modules[0];
  int i = 0;
                                                                                      
  assert(name);
                                                                                      
  while (ptr[i] != NULL)
    {
      if (!ptr[i]->config_module_name)
        {
          cerebrod_err_debug("static config module index '%d' does not contain name", i);
          continue;
        }
      if (!strcmp(ptr[i]->config_module_name, name))
        return ptr[i];
      i++;
    }
  
  return NULL;
}

struct cerebrod_clusterlist_module_info *
cerebrod_find_static_clusterlist_module(char *name)
{
  struct cerebrod_clusterlist_module_info **ptr = &static_clusterlist_modules[0];
  int i = 0;
  
  assert(name);
  
  while (ptr[i] != NULL)
    {
      if (!ptr[i]->clusterlist_module_name)
        {
          cerebrod_err_debug("static clusterlist module index '%d' does not contain name", i);
          continue;
        }
      if (!strcmp(ptr[i]->clusterlist_module_name, name))
        return ptr[i];
      i++;
    }
                                                                                      
  return NULL;
}

#endif /* WITH_STATIC_MODULES */
