/*****************************************************************************\
 *  $Id: cerebrod_clusterlist.h,v 1.9 2005-03-24 01:29:21 achu Exp $
\*****************************************************************************/

#ifndef _CEREBROD_CLUSTERLIST_H
#define _CEREBROD_CLUSTERLIST_H

/*  
 * cerebrod_clusterlist_setup
 *
 * load clusterlist module.  search for clusterlist module to load if
 * necessary
 *
 * Return 0 on success, -1 on error
 */
int cerebrod_clusterlist_setup(void);

/*  
 * cerebrod_clusterlist_cleanup
 *
 * cleanup clusterlist module state
 *
 * Return 0 on success, -1 on error
 */
int cerebrod_clusterlist_cleanup(void);

/* 
 * cerebrod_clusterlist_module_name
 *
 * Return clusterlist module name
 */
char *cerebrod_clusterlist_module_name(void);

/* 
 * cerebrod_clusterlist_parse_options
 * 
 * call clusterlist module parse options function
 */
int cerebrod_clusterlist_parse_options(void);

/* 
 * cerebrod_clusterlist_init
 * 
 * call clusterlist module init function
 */
int cerebrod_clusterlist_init(void);

/* 
 * cerebrod_clusterlist_finish
 * 
 * call clusterlist module parse finish function
 */
int cerebrod_clusterlist_finish(void);

/* 
 * cerebrod_clusterlist_get_all_nodes
 * 
 * call clusterlist module get all nodes function
 */
int cerebrod_clusterlist_get_all_nodes(char **nodes, unsigned int nodeslen);

/* 
 * cerebrod_clusterlist_numnodes
 * 
 * call clusterlist module numnodes function
 */
int cerebrod_clusterlist_numnodes(void);

/* 
 * cerebrod_clusterlist_node_in_cluster
 * 
 * call clusterlist module node in cluster function
 */
int cerebrod_clusterlist_node_in_cluster(char *node);

/* 
 * cerebrod_clusterlist_get_nodename
 * 
 * call clusterlist module get nodename function
 */
int cerebrod_clusterlist_get_nodename(char *node, char *buf, unsigned int buflen);

#endif /* _CEREBROD_CLUSTERLIST_H */
