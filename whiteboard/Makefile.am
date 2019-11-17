include $(top_srcdir)/Makefile.tool.am

EXTRA_DIST = docs/wb-manual.xml

#----------------------------------------------------------------------------
# whiteboard-<platform>
#----------------------------------------------------------------------------

noinst_PROGRAMS  = whiteboard-@VGCONF_ARCH_PRI@-@VGCONF_OS@
if VGCONF_HAVE_PLATFORM_SEC
noinst_PROGRAMS += whiteboard-@VGCONF_ARCH_SEC@-@VGCONF_OS@
endif

WHITEBOARD_SOURCES_COMMON = wb_main.c

whiteboard_@VGCONF_ARCH_PRI@_@VGCONF_OS@_SOURCES      = \
	$(WHITEBOARD_SOURCES_COMMON)
whiteboard_@VGCONF_ARCH_PRI@_@VGCONF_OS@_CPPFLAGS     = \
	$(AM_CPPFLAGS_@VGCONF_PLATFORM_PRI_CAPS@)
whiteboard_@VGCONF_ARCH_PRI@_@VGCONF_OS@_CFLAGS       = $(LTO_CFLAGS) \
	$(AM_CFLAGS_@VGCONF_PLATFORM_PRI_CAPS@)
whiteboard_@VGCONF_ARCH_PRI@_@VGCONF_OS@_DEPENDENCIES = \
	$(TOOL_DEPENDENCIES_@VGCONF_PLATFORM_PRI_CAPS@)
whiteboard_@VGCONF_ARCH_PRI@_@VGCONF_OS@_LDADD        = \
	$(TOOL_LDADD_@VGCONF_PLATFORM_PRI_CAPS@)
whiteboard_@VGCONF_ARCH_PRI@_@VGCONF_OS@_LDFLAGS      = \
	$(TOOL_LDFLAGS_@VGCONF_PLATFORM_PRI_CAPS@)
whiteboard_@VGCONF_ARCH_PRI@_@VGCONF_OS@_LINK = \
	$(top_builddir)/coregrind/link_tool_exe_@VGCONF_OS@ \
	@VALT_LOAD_ADDRESS_PRI@ \
	$(LINK) \
	$(whiteboard_@VGCONF_ARCH_PRI@_@VGCONF_OS@_CFLAGS) \
	$(whiteboard_@VGCONF_ARCH_PRI@_@VGCONF_OS@_LDFLAGS)

if VGCONF_HAVE_PLATFORM_SEC
whiteboard_@VGCONF_ARCH_SEC@_@VGCONF_OS@_SOURCES      = \
	$(NONE_SOURCES_COMMON)
whiteboard_@VGCONF_ARCH_SEC@_@VGCONF_OS@_CPPFLAGS     = \
	$(AM_CPPFLAGS_@VGCONF_PLATFORM_SEC_CAPS@)
whiteboard_@VGCONF_ARCH_SEC@_@VGCONF_OS@_CFLAGS       = $(LTO_CFLAGS) \
	$(AM_CFLAGS_@VGCONF_PLATFORM_SEC_CAPS@)
whiteboard_@VGCONF_ARCH_SEC@_@VGCONF_OS@_DEPENDENCIES = \
	$(TOOL_DEPENDENCIES_@VGCONF_PLATFORM_SEC_CAPS@)
whiteboard_@VGCONF_ARCH_SEC@_@VGCONF_OS@_LDADD        = \
	$(TOOL_LDADD_@VGCONF_PLATFORM_SEC_CAPS@)
whiteboard_@VGCONF_ARCH_SEC@_@VGCONF_OS@_LDFLAGS      = \
	$(TOOL_LDFLAGS_@VGCONF_PLATFORM_SEC_CAPS@)
whiteboard_@VGCONF_ARCH_SEC@_@VGCONF_OS@_LINK = \
	$(top_builddir)/coregrind/link_tool_exe_@VGCONF_OS@ \
	@VALT_LOAD_ADDRESS_SEC@ \
	$(LINK) \
	$(whiteboard_@VGCONF_ARCH_SEC@_@VGCONF_OS@_CFLAGS) \
	$(whiteboard_@VGCONF_ARCH_SEC@_@VGCONF_OS@_LDFLAGS)
endif


