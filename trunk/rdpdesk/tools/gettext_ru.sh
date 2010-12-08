#!/bin/sh

PO_FILE="ru.po"
CPP_FILE_LIST=`find ../src -name '*.*pp' -print | grep -v AxVncViewer |grep -v /res | grep -v vnc_unixsrc`

./gettext.sh
if ([ -e ../po/${PO_FILE}  ]) then
{
    cp ../po/${PO_FILE} ../po/${PO_FILE}.bak
    xgettext -d rdpdesk -s -j --keyword=_ -p ../po -o ${PO_FILE}  ${CPP_FILE_LIST}
    perl -ne 's/\"Content-Type\: text\/plain; charset=UTF-8/\"Content-Type\: text\/plain; charset=koi8-r/g' -pi ../po/${PO_FILE}
    iconv ../po/${PO_FILE} -t koi8-r > ../po/${PO_FILE}_temp
    mv ../po/${PO_FILE}_temp ../po/${PO_FILE}
} else {
#    xgettext -d rdpdesk -s --keyword=_ -p ../po -o ${PO_FILE}  ${CPP_FILE_LIST}
    #perl -ne 's/Content/111111/g' -pi ../po/${PO_FILE}
    echo
} fi

