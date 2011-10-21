/*****************************************************************************\
 *  $Id: cerebro_config_util.h,v 1.8 2010-02-02 01:01:20 chu11 Exp $
 *****************************************************************************
 *  Copyright (C) 2007-2011 Lawrence Livermore National Security, LLC.
 *  Copyright (C) 2005-2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Albert Chu <chu11@llnl.gov>.
 *  UCRL-CODE-155989 All rights reserved.
 *
 *  This file is part of Cerebro, a collection of cluster monitoring
 *  tools and libraries.  For details, see
 *  <http://www.llnl.gov/linux/cerebro/>.
 *
 *  Cerebro is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  Cerebro is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Cerebro.  If not, see <http://www.gnu.org/licenses/>.
\*****************************************************************************/

#ifndef _CEREBRO_CONFIG_UTIL_H
#define _CEREBRO_CONFIG_UTIL_H

#include "cerebro.h"

/* 
 * _cerebro_load_config
 *
 * Read and load config file data
 *
 * Returns 0 on success, -1 on error
 */
int _cerebro_load_config(cerebro_t handle);

/* 
 * _cerebro_unload_config
 *
 * Unload config file info
 *
 * Returns 0 on success, -1 on error
 */
int _cerebro_unload_config(cerebro_t handle);

#endif /* _CEREBRO_CONFIG_UTIL_H */
