FORMS         += adio_pin.ui
FORMS         += ci2c_io.ui
FORMS         += rgbi2c.ui

HEADERS      += heartbeat.h
#HEADERS      += tftpc.h

SOURCES      += main.cpp
SOURCES      += heartbeat.cpp
#SOURCES      += tftpc.cpp


SOURCES += edlg.cpp
HEADERS += edlg.h
HEADERS += led.h
SOURCES += led.cpp

SOURCES += highlighter.cpp
HEADERS += highlighter.h

SOURCES += adio_pin.cpp
HEADERS += adio_pin.h
SOURCES += ci2c_io.cpp
HEADERS += ci2c_io.h
SOURCES += rgbi2c.cpp
HEADERS += rgbi2c.h

#RP= ../../../scratch1
RP= ../..

RC_FILE       = iwscope.rc
RESOURCES     = communicator.qrc
#SOURCES += images

DESTDIR      = bin
unix {
MOC_DIR      = /tmp/$${TARGET}
RCC_DIR      = /tmp/$${TARGET}
OBJECTS_DIR  = /tmp/$${TARGET}
}

win {
MOC_DIR      = c:/tmp/$${TARGET}
RCC_DIR      = c:/tmp/$${TARGET}
OBJECTS_DIR  = c:/tmp/$${TARGET}
}



QT       += core gui widgets network

# install
target.path = $$[QT_INSTALL_EXAMPLES]/dialogs/standarddialogs
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/dialogs/standarddialogs
INSTALLS += target sources

CONFIG       += wwwidgets

unix {
    DEFINES += LINUX_WAY
    #DEFINES += FORCE_STATIC_LIB
    QWT_LOCATION = /usr/local/qwt-6.0.2
    INCLUDEPATH += $${QWT_LOCATION}/include
    INCLUDEPATH += /usr/include
    INCLUDEPATH += /usr/include/qt4/wwWidgets
    LIBS += -L$${QWT_LOCATION}/lib -lqwt
    LIBS += -L$${QWT_LOCATION}/lib -lqwt
    LIBS += -lgsl
    LIBS += -lgslcblas
}

win32 {
DEFINES += WINDOWS_WAY
DEFINES += _WIN32
QWT_LOCATION = C:/Qt/4.8.7/extra/qwt-6.0.2
INCLUDEPATH += $${QWT_LOCATION}/src
LIBS += -L$${QWT_LOCATION}/lib -lqwt
}


INCLUDEPATH  += .
INCLUDEPATH  += ../udplib
INCLUDEPATH  += led
INCLUDEPATH  += ../common
INCLUDEPATH  += ../../ext_common
INCLUDEPATH  += ../../../common/endpoints
INCLUDEPATH  += ../../../../git/emulator_v0/lib/include

FILE_PATH     = ../common

LIBS         += -lm
LIBS        += -L../udplib/bin -ludp
INCLUDEPATH += ../extudp


TARGET   = heartbeat

CONFIG       += wwwidgets

unix {
QWT_LOCATION = /usr/local/qwt-6.0.2
DEFINES      += LINUX_WAY
INCLUDEPATH  += $${QWT_LOCATION}/include
LIBS += -L$${QWT_LOCATION}/lib -lqwt
}

win32 {
DEFINES += WINDOWS_WAY
QWT_LOCATION = C:/Qt/4.8.7/extra/qwt-6.0.2
INCLUDEPATH += $${QWT_LOCATION}/src
LIBS += -L$${QWT_LOCATION}/lib -lqwt
}

unix {
PRE_TARGETDEPS       = ../udplib/bin/udp.so
#PRE_TARGETDEPS      += ../../../common/cscript/run_cscr

Ext_Lib.target     = ../udplib/bin/udp.so
Ext_Lib.commands   = cd ../udplib && Qmake
Ext_Lib.depends    = ../udplib/proextlib.pro
QMAKE_EXTRA_TARGETS += Ext_Lib

#run_cscr.target     =    ../../common/cscript/bin/run_cscr
#run_cscr.commands   = cd ../../common/cscript && Qmake
#run_cscr.depends    =    ../../common/cscript/project.pro
#QMAKE_EXTRA_TARGETS +=   ../../common/cscript/run_cscr
}

#QMAKE_POST_LINK += "cp ../../../common/cscript/bin/run_cscr bin"

QMAKE_CXXFLAGS_RELEASE  = -O0

#HEADERS      += cli_cmds.h
#SOURCES      += cli_cmds.c

#INCLUDEPATH += ../../../common/cli

#HEADERS += ../../../common/cli/cli.h
#SOURCES += ../../../common/cli/cli.cpp
