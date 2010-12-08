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
    gui/helpbrowser.cpp \
    proto/network_helper.cpp \
    proto/RDPConnection_nix.cpp \
    proto/RDPOptionsDialog.cpp \
    proto/BasicConnection.cpp \
    proto/ICAConnection_nix.cpp \
    proto/ICAOptionsDialog.cpp \
    proto/RFBConnection_nix.cpp \
    proto/RFBOptionsDialog.cpp \
    proto/FRDPConnection_nix.cpp \
    proto/FRDPOptionsDialog.cpp \
    proto/FRDPKeyboard.cpp \
    modules/autoupdate/autoupdate.cpp \
    modules/CatcherWindowID/CatcherWindowID.cpp 

#rdpdesk_LDADD = 
#$(WX_LIBS) $(SSL_LIBS) @LIBS@
#rdpdesk_LDADD = 
rdpdesk_CXXFLAGS = @CXXFLAGS@ $(WX_CXXFLAGS) $(LOCAL_INCLUDES) $(GTK2_INCLUDE) -DMITSHM 
rdpdesk_CFLAGS = @CXXFLAGS@ $(WX_CXXFLAGS) $(LOCAL_INCLUDES) $(GTK2_INCLUDE) -DMITSHM 
#-lconfig_dialog $(WX_LIBS)
#
INCLUDES = -Igui \
	-Igui/config \
	-Iproto \
	-Iinclude \
	-Iargsresources.c \
	-Iproto/vnc_unixsrc/include \
	-Iproto/vnc_unixsrc/vncviewer  \
	-Iproto/vnc_unixsrc/libvncauth \
	-Imodules/autoupdate \
	-Imodules/CatcherWindowID

rdpdesk_LDADD = $(WX_LIBS) $(GTK2_LIBS) $(SSL_LIBS) -ljpeg -lz -lXaw
#AM_LDFLAGS = `wx-config --libs`

SUBDIRS = proto/vnc_unixsrc/libvncauth proto/vnc_unixsrc/vncviewer 