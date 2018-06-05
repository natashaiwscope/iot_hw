FORMS         += adio_pin.ui
FORMS         += ci2c_io.ui
FORMS         += rgbi2c.ui
FORMS         += humidity.ui

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

SOURCES += humidity.cpp
HEADERS += humidity.h

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


INCLUDEPATH  += ../../lib/include
INCLUDEPATH  += .
FILE_PATH     = ../common

LIBS         += -lm


TARGET   = nc_io

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

#QMAKE_POST_LINK += "cp ../../../common/cscript/bin/run_cscr bin"

QMAKE_CXXFLAGS_RELEASE  = -O0


message ("Host = " $${QMAKE_HOST.arch})

unix {
contains(QMAKE_HOST.arch, x86_64):{
    LIBS        += -L../../lib/linux64 -ludp
}

contains(QMAKE_HOST.arch, i686):{
    LIBS        += -L../../lib/linux32 -ludp
}

contains(QMAKE_HOST.arch, arm):{
    LIBS        += -L../../lib/arm -ludp
}

}

win32 {
    LIBS        += -L../../lib/win32 -ludp
}


unix {
    DEFINES += LINUX_WAY
}


