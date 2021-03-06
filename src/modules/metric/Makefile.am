##*****************************************************************************
## $Id: Makefile.am,v 1.26 2008-03-28 16:18:58 chu11 Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

noinst_HEADERS = \
	cerebro_metric_common.h \
	cerebro_metric_loadavg.h \
	cerebro_metric_memory.h \
	cerebro_metric_network.h

if WITH_GNU_LD
METRIC_VERSION_SCRIPT = $(srcdir)/metric.map
METRIC_OTHER_FLAGS = -Wl,--version-script=$(METRIC_VERSION_SCRIPT)
endif

if WITH_BOOTTIME
BOOTTIME_METRIC_MODULE = cerebro_metric_boottime.la
BOOTTIME_STATIC_SOURCES = cerebro_metric_boottime.c
endif

if WITH_LOADAVG
LOADAVG_STATIC_SOURCES = cerebro_metric_loadavg.c

LOADAVG1_METRIC_MODULE = cerebro_metric_loadavg1.la
LOADAVG1_STATIC_SOURCES = cerebro_metric_loadavg1.c

LOADAVG5_METRIC_MODULE = cerebro_metric_loadavg5.la
LOADAVG5_STATIC_SOURCES = cerebro_metric_loadavg5.c

LOADAVG15_METRIC_MODULE = cerebro_metric_loadavg15.la
LOADAVG15_STATIC_SOURCES = cerebro_metric_loadavg15.c
endif

if WITH_MEMORY
MEMORY_STATIC_SOURCES = cerebro_metric_memory.c

MEMTOTAL_METRIC_MODULE = cerebro_metric_memtotal.la
MEMTOTAL_STATIC_SOURCES = cerebro_metric_memtotal.c

MEMUSED_METRIC_MODULE = cerebro_metric_memused.la
MEMUSED_STATIC_SOURCES = cerebro_metric_memused.c

MEMFREE_METRIC_MODULE = cerebro_metric_memfree.la
MEMFREE_STATIC_SOURCES = cerebro_metric_memfree.c

SWAPTOTAL_METRIC_MODULE = cerebro_metric_swaptotal.la
SWAPTOTAL_STATIC_SOURCES = cerebro_metric_swaptotal.c

SWAPUSED_METRIC_MODULE = cerebro_metric_swapused.la
SWAPUSED_STATIC_SOURCES = cerebro_metric_swapused.c

SWAPFREE_METRIC_MODULE = cerebro_metric_swapfree.la
SWAPFREE_STATIC_SOURCES = cerebro_metric_swapfree.c
endif

if WITH_NETWORK
NETWORK_STATIC_SOURCES = cerebro_metric_network.c

BYTESIN_METRIC_MODULE = cerebro_metric_bytesin.la
BYTESIN_STATIC_SOURCES = cerebro_metric_bytesin.c

BYTESOUT_METRIC_MODULE = cerebro_metric_bytesout.la
BYTESOUT_STATIC_SOURCES = cerebro_metric_bytesout.c

PACKETSIN_METRIC_MODULE = cerebro_metric_packetsin.la
PACKETSIN_STATIC_SOURCES = cerebro_metric_packetsin.c

PACKETSOUT_METRIC_MODULE = cerebro_metric_packetsout.la
PACKETSOUT_STATIC_SOURCES = cerebro_metric_packetsout.c

RXERRS_METRIC_MODULE = cerebro_metric_rxerrs.la
RXERRS_STATIC_SOURCES = cerebro_metric_rxerrs.c

TXERRS_METRIC_MODULE = cerebro_metric_txerrs.la
TXERRS_STATIC_SOURCES = cerebro_metric_txerrs.c
endif

if WITH_SLURM_STATE
SLURM_STATE_METRIC_MODULE = cerebro_metric_slurm_state.la
SLURM_STATE_STATIC_SOURCES = cerebro_metric_slurm_state.c
endif

if WITH_SHUTDOWN
SHUTDOWN_METRIC_MODULE = cerebro_metric_shutdown.la
SHUTDOWN_STATIC_SOURCES = cerebro_metric_shutdown.c
endif

if WITH_STATIC_MODULES
noinst_LTLIBRARIES = libstaticmetricmodules.la
else
modulelibdir = $(CEREBRO_MODULE_DIR)
modulelib_LTLIBRARIES = $(BOOTTIME_METRIC_MODULE) \
			$(LOADAVG1_METRIC_MODULE) \
			$(LOADAVG5_METRIC_MODULE) \
			$(LOADAVG15_METRIC_MODULE) \
			$(MEMTOTAL_METRIC_MODULE) \
			$(MEMUSED_METRIC_MODULE) \
			$(MEMFREE_METRIC_MODULE) \
			$(SWAPTOTAL_METRIC_MODULE) \
			$(SWAPUSED_METRIC_MODULE) \
			$(SWAPFREE_METRIC_MODULE) \
			$(BYTESIN_METRIC_MODULE) \
			$(BYTESOUT_METRIC_MODULE) \
			$(PACKETSIN_METRIC_MODULE) \
			$(PACKETSOUT_METRIC_MODULE) \
			$(RXERRS_METRIC_MODULE) \
			$(TXERRS_METRIC_MODULE) \
			$(SLURM_STATE_METRIC_MODULE) \
			$(SHUTDOWN_METRIC_MODULE)
endif

MODULE_FLAGS = -module -avoid-version

libstaticmetricmodules_la_SOURCES = $(BOOTTIME_STATIC_SOURCES) \
				    $(LOADAVG_STATIC_SOURCES) \
				    $(LOADAVG1_STATIC_SOURCES) \
				    $(LOADAVG5_STATIC_SOURCES) \
				    $(LOADAVG15_STATIC_SOURCES) \
				    $(MEMORY_STATIC_SOURCES) \
				    $(MEMTOTAL_STATIC_SOURCES) \
				    $(MEMUSED_STATIC_SOURCES) \
				    $(MEMFREE_STATIC_SOURCES) \
				    $(SWAPTOTAL_STATIC_SOURCES) \
				    $(SWAPUSED_STATIC_SOURCES) \
				    $(SWAPFREE_STATIC_SOURCES) \
				    $(NETWORK_STATIC_SOURCES) \
				    $(BYTESIN_STATIC_SOURCES) \
				    $(BYTESOUT_STATIC_SOURCES) \
				    $(PACKETSIN_STATIC_SOURCES) \
				    $(PACKETSOUT_STATIC_SOURCES) \
				    $(RXERRS_STATIC_SOURCES) \
				    $(TXERRS_STATIC_SOURCES) \
				    $(SLURM_STATE_STATIC_SOURCES) \
				    $(SHUTDOWN_STATIC_SOURCES) \
				    cerebro_metric_common.c
libstaticmetricmodules_la_CFLAGS = -I$(srcdir)/../../libs/include \
			     	   -I$(srcdir)/../../libs/common \
			     	   -I$(srcdir)/../../libs/debug \
			     	   -I$(srcdir)/../../libs/util
libstaticmetricmodules_la_LDFLAGS = $(METRIC_OTHER_FLAGS)

cerebro_metric_boottime_la_SOURCES = cerebro_metric_boottime.c \
				     cerebro_metric_common.c
cerebro_metric_boottime_la_LDFLAGS = $(MODULE_FLAGS) \
			             $(METRIC_OTHER_FLAGS)
cerebro_metric_boottime_la_CFLAGS  = -I$(srcdir)/../../libs/include \
				     -I$(srcdir)/../../libs/debug
cerebro_metric_boottime_la_LIBADD  = ../../libs/debug/libdebug.la \
				     ../../libs/cerebro_error/libcerebro_error.la

cerebro_metric_loadavg1_la_SOURCES = cerebro_metric_loadavg1.c \
				     cerebro_metric_loadavg.c \
				     cerebro_metric_common.c
cerebro_metric_loadavg1_la_LDFLAGS = $(MODULE_FLAGS) \
			             $(METRIC_OTHER_FLAGS)
cerebro_metric_loadavg1_la_CFLAGS  = -I$(srcdir)/../../libs/include \
				     -I$(srcdir)/../../libs/debug
cerebro_metric_loadavg1_la_LIBADD  = ../../libs/debug/libdebug.la \
				     ../../libs/cerebro_error/libcerebro_error.la

cerebro_metric_loadavg5_la_SOURCES = cerebro_metric_loadavg5.c \
				     cerebro_metric_loadavg.c \
				     cerebro_metric_common.c
cerebro_metric_loadavg5_la_LDFLAGS = $(MODULE_FLAGS) \
			             $(METRIC_OTHER_FLAGS)
cerebro_metric_loadavg5_la_CFLAGS  = -I$(srcdir)/../../libs/include \
				     -I$(srcdir)/../../libs/debug
cerebro_metric_loadavg5_la_LIBADD  = ../../libs/debug/libdebug.la \
				     ../../libs/cerebro_error/libcerebro_error.la

cerebro_metric_loadavg15_la_SOURCES = cerebro_metric_loadavg15.c \
				      cerebro_metric_loadavg.c \
				      cerebro_metric_common.c
cerebro_metric_loadavg15_la_LDFLAGS = $(MODULE_FLAGS) \
			              $(METRIC_OTHER_FLAGS)
cerebro_metric_loadavg15_la_CFLAGS  = -I$(srcdir)/../../libs/include \
				      -I$(srcdir)/../../libs/debug
cerebro_metric_loadavg15_la_LIBADD  = ../../libs/debug/libdebug.la \
				      ../../libs/cerebro_error/libcerebro_error.la

cerebro_metric_memtotal_la_SOURCES = cerebro_metric_memtotal.c \
				     cerebro_metric_memory.c \
				     cerebro_metric_common.c
cerebro_metric_memtotal_la_LDFLAGS = $(MODULE_FLAGS) \
				     $(METRIC_OTHER_FLAGS)
cerebro_metric_memtotal_la_CFLAGS  = -I$(srcdir)/../../libs/include \
				     -I$(srcdir)/../../libs/debug
cerebro_metric_memtotal_la_LIBADD  = ../../libs/debug/libdebug.la \
				      ../../libs/cerebro_error/libcerebro_error.la

cerebro_metric_memused_la_SOURCES = cerebro_metric_memused.c \
				    cerebro_metric_memory.c \
				    cerebro_metric_common.c
cerebro_metric_memused_la_LDFLAGS = $(MODULE_FLAGS) \
				    $(METRIC_OTHER_FLAGS)
cerebro_metric_memused_la_CFLAGS  = -I$(srcdir)/../../libs/include \
				    -I$(srcdir)/../../libs/debug
cerebro_metric_memused_la_LIBADD  = ../../libs/debug/libdebug.la \
				    ../../libs/cerebro_error/libcerebro_error.la

cerebro_metric_memfree_la_SOURCES = cerebro_metric_memfree.c \
				    cerebro_metric_memory.c \
				    cerebro_metric_common.c
cerebro_metric_memfree_la_LDFLAGS = $(MODULE_FLAGS) \
				    $(METRIC_OTHER_FLAGS)
cerebro_metric_memfree_la_CFLAGS  = -I$(srcdir)/../../libs/include \
				    -I$(srcdir)/../../libs/debug
cerebro_metric_memfree_la_LIBADD  = ../../libs/debug/libdebug.la \
				    ../../libs/cerebro_error/libcerebro_error.la

cerebro_metric_swaptotal_la_SOURCES = cerebro_metric_swaptotal.c \
				      cerebro_metric_memory.c \
				      cerebro_metric_common.c
cerebro_metric_swaptotal_la_LDFLAGS = $(MODULE_FLAGS) \
				      $(METRIC_OTHER_FLAGS)
cerebro_metric_swaptotal_la_CFLAGS  = -I$(srcdir)/../../libs/include \
				      -I$(srcdir)/../../libs/debug
cerebro_metric_swaptotal_la_LIBADD  = ../../libs/debug/libdebug.la \
				      ../../libs/cerebro_error/libcerebro_error.la

cerebro_metric_swapused_la_SOURCES = cerebro_metric_swapused.c \
				     cerebro_metric_memory.c \
				     cerebro_metric_common.c
cerebro_metric_swapused_la_LDFLAGS = $(MODULE_FLAGS) \
				     $(METRIC_OTHER_FLAGS)
cerebro_metric_swapused_la_CFLAGS  = -I$(srcdir)/../../libs/include \
				     -I$(srcdir)/../../libs/debug
cerebro_metric_swapused_la_LIBADD  = ../../libs/debug/libdebug.la \
				     ../../libs/cerebro_error/libcerebro_error.la

cerebro_metric_swapfree_la_SOURCES = cerebro_metric_swapfree.c \
				     cerebro_metric_memory.c \
				     cerebro_metric_common.c
cerebro_metric_swapfree_la_LDFLAGS = $(MODULE_FLAGS) \
				     $(METRIC_OTHER_FLAGS)
cerebro_metric_swapfree_la_CFLAGS  = -I$(srcdir)/../../libs/include \
				     -I$(srcdir)/../../libs/debug
cerebro_metric_swapfree_la_LIBADD  = ../../libs/debug/libdebug.la \
				     ../../libs/cerebro_error/libcerebro_error.la

cerebro_metric_bytesin_la_SOURCES = cerebro_metric_bytesin.c \
				    cerebro_metric_network.c \
				    cerebro_metric_common.c
cerebro_metric_bytesin_la_LDFLAGS = $(MODULE_FLAGS) \
				    $(METRIC_OTHER_FLAGS)
cerebro_metric_bytesin_la_CFLAGS  = -I$(srcdir)/../../libs/include \
				    -I$(srcdir)/../../libs/debug
cerebro_metric_bytesin_la_LIBADD  = ../../libs/debug/libdebug.la \
				    ../../libs/cerebro_error/libcerebro_error.la

cerebro_metric_bytesout_la_SOURCES = cerebro_metric_bytesout.c \
				     cerebro_metric_network.c \
				     cerebro_metric_common.c
cerebro_metric_bytesout_la_LDFLAGS = $(MODULE_FLAGS) \
				     $(METRIC_OTHER_FLAGS)
cerebro_metric_bytesout_la_CFLAGS  = -I$(srcdir)/../../libs/include \
				     -I$(srcdir)/../../libs/debug
cerebro_metric_bytesout_la_LIBADD  = ../../libs/debug/libdebug.la \
				     ../../libs/cerebro_error/libcerebro_error.la

cerebro_metric_packetsin_la_SOURCES = cerebro_metric_packetsin.c \
				      cerebro_metric_network.c \
				      cerebro_metric_common.c
cerebro_metric_packetsin_la_LDFLAGS = $(MODULE_FLAGS) \
				      $(METRIC_OTHER_FLAGS)
cerebro_metric_packetsin_la_CFLAGS  = -I$(srcdir)/../../libs/include \
				      -I$(srcdir)/../../libs/debug
cerebro_metric_packetsin_la_LIBADD  = ../../libs/debug/libdebug.la \
				      ../../libs/cerebro_error/libcerebro_error.la

cerebro_metric_packetsout_la_SOURCES = cerebro_metric_packetsout.c \
				       cerebro_metric_network.c \
				       cerebro_metric_common.c
cerebro_metric_packetsout_la_LDFLAGS = $(MODULE_FLAGS) \
				       $(METRIC_OTHER_FLAGS)
cerebro_metric_packetsout_la_CFLAGS  = -I$(srcdir)/../../libs/include \
				       -I$(srcdir)/../../libs/debug
cerebro_metric_packetsout_la_LIBADD  = ../../libs/debug/libdebug.la \
				       ../../libs/cerebro_error/libcerebro_error.la

cerebro_metric_rxerrs_la_SOURCES = cerebro_metric_rxerrs.c \
				   cerebro_metric_network.c \
				   cerebro_metric_common.c
cerebro_metric_rxerrs_la_LDFLAGS = $(MODULE_FLAGS) \
				   $(METRIC_OTHER_FLAGS)
cerebro_metric_rxerrs_la_CFLAGS  = -I$(srcdir)/../../libs/include \
				   -I$(srcdir)/../../libs/debug
cerebro_metric_rxerrs_la_LIBADD  = ../../libs/debug/libdebug.la \
				   ../../libs/cerebro_error/libcerebro_error.la

cerebro_metric_txerrs_la_SOURCES = cerebro_metric_txerrs.c \
				   cerebro_metric_network.c \
				   cerebro_metric_common.c
cerebro_metric_txerrs_la_LDFLAGS = $(MODULE_FLAGS) \
				   $(METRIC_OTHER_FLAGS)
cerebro_metric_txerrs_la_CFLAGS  = -I$(srcdir)/../../libs/include \
				   -I$(srcdir)/../../libs/debug
cerebro_metric_txerrs_la_LIBADD  = ../../libs/debug/libdebug.la \
				   ../../libs/cerebro_error/libcerebro_error.la

cerebro_metric_slurm_state_la_SOURCES = cerebro_metric_slurm_state.c \
				        cerebro_metric_common.c
cerebro_metric_slurm_state_la_LDFLAGS = $(MODULE_FLAGS) \
					$(METRIC_OTHER_FLAGS)
cerebro_metric_slurm_state_la_CFLAGS  = -I$(srcdir)/../../libs/include \
				        -I$(srcdir)/../../libs/debug
cerebro_metric_slurm_state_la_LIBADD  = ../../libs/debug/libdebug.la \
				        ../../libs/cerebro_error/libcerebro_error.la

cerebro_metric_shutdown_la_SOURCES = cerebro_metric_shutdown.c \
				     cerebro_metric_common.c
cerebro_metric_shutdown_la_LDFLAGS = $(MODULE_FLAGS) \
				     $(METRIC_OTHER_FLAGS)
cerebro_metric_shutdown_la_CFLAGS  = -I$(srcdir)/../../libs/include \
				     -I$(srcdir)/../../libs/debug
cerebro_metric_shutdown_la_LIBADD  = ../../libs/debug/libdebug.la \
				     ../../libs/cerebro_error/libcerebro_error.la

../../libs/common/libconffile.la : force-dependency-check
	@cd `dirname $@` && make `basename $@`

../../libs/common/libfd.la : force-dependency-check
	@cd `dirname $@` && make `basename $@`

../../libs/cerebro_error/libcerebro_error.la : force-dependency-check
	@cd `dirname $@` && make `basename $@`

../../libs/debug/libdebug.la : force-dependency-check
	@cd `dirname $@` && make `basename $@`

../../libs/util/libutil_network.la : force-dependency-check
	@cd `dirname $@` && make `basename $@`

force-dependency-check:

EXTRA_DIST = metric.map
