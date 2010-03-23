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
    proto/RDPConnection_nix.cpp \
    proto/RDPOptionsDialog.cpp \
    proto/BasicConnection.cpp \
    proto/ICAConnection_nix.cpp \
    proto/ICAOptionsDialog.cpp


#rdpdesk_LDADD = 
#$(WX_LIBS) $(SSL_LIBS) @LIBS@
#rdpdesk_LDADD = 
rdpdesk_CXXFLAGS = @CXXFLAGS@ $(WX_CXXFLAGS) $(LOCAL_INCLUDES) $(GTK2_INCLUDE)
#-lconfig_dialog $(WX_LIBS)
#
INCLUDES = -Igui -Igui/config -Iproto -Iinclude
rdpdesk_LDADD = $(WX_LIBS) $(GTK2_LIBS) $(SSL_LIBS)
#AM_LDFLAGS = `wx-config --libs`
