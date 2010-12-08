#!/bin/sh

PO_FILE="rdpdesk.pot"
CPP_FILE_LIST=`find ../src -name '*.*pp' -print | grep -v AxVncViewer |grep -v /res | grep -v vnc_unixsrc`

if ([ -e ../po/${PO_FILE}  ]) then
{
    xgettext -d rdpdesk -s -j --keyword=_ -p ../po -o ${PO_FILE}  ${CPP_FILE_LIST}
} else {
    xgettext -d rdpdesk -s --keyword=_ -p ../po -o ${PO_FILE}  ${CPP_FILE_LIST}
} fi

