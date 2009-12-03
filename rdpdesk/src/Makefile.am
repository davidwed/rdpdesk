#SUBDIR = gui proto

bin_PROGRAMS = rdpdesk
rdpdesk_SOURCES=main.cpp \
    bencoded.cpp \
    locale_helper.cpp \
    gui/main_window.cpp \
    gui/config/rdp_dialogs.cpp \
    gui/config/fastconn_dialog.cpp \
    gui/settings_dialog.cpp \
    gui/splitter.cpp \
    gui/small_shoot.cpp \
    gui/tree_group.cpp \
    proto/network_helper.cpp \
    proto/wxRDP_nix.cpp
#rdp_clients_LDADD = 
#$(WX_LIBS) $(SSL_LIBS) @LIBS@
#rdp_clients_LDADD = 
rdpdesk_CXXFLAGS = @CXXFLAGS@ $(WX_CXXFLAGS) $(LOCAL_INCLUDES) $(GTK2_INCLUDE)
#-lconfig_dialog $(WX_LIBS)
#
INCLUDES = -Igui -Igui/config -Iproto
rdpdesk_LDADD = $(WX_LIBS) $(GTK2_LIBS) $(SSL_LIBS)
#AM_LDFLAGS = `wx-config --libs`
