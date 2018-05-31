
TARGET = hyperTerm
FORMS        += settingsdialog.ui
SOURCES += csql_db.cpp
HEADERS += csql_db.h

HEADERS      += heartbeat.h

SOURCES      += main.cpp
SOURCES      += heartbeat.cpp

SOURCES += console.cpp
HEADERS += console.h

HEADERS += led.h
SOURCES += led.cpp

SOURCES += codeeditor.cpp
HEADERS += codeeditor.h

SOURCES += highlighter.cpp
HEADERS += highlighter.h

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

win32 {
DEFINES += WINDOWS_WAY
QWT_LOCATION = C:/Qt/4.8.7/extra/qwt-6.0.2
INCLUDEPATH += $${QWT_LOCATION}/src
LIBS += -L$${QWT_LOCATION}/lib -lqwt
}


QT       += core gui widgets network
QT += sql

# install
target.path = $$[QT_INSTALL_EXAMPLES]/dialogs/standarddialogs
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/dialogs/standarddialogs
INSTALLS += target sources

unix {
    DEFINES += LINUX_WAY
    #DEFINES += FORCE_STATIC_LIB
    QWT_LOCATION = /usr/local/qwt-6.0.2
    INCLUDEPATH += $${QWT_LOCATION}/include
    INCLUDEPATH += /usr/include
    LIBS += -L$${QWT_LOCATION}/lib -lqwt
    LIBS += -L$${QWT_LOCATION}/lib -lqwt
    LIBS += -lgsl
    LIBS += -lgslcblas
}



INCLUDEPATH  += .
INCLUDEPATH  += led
#INCLUDEPATH  += ../common
#INCLUDEPATH  += ../../../common/endpoints
#INCLUDEPATH  += ../../ext_common

FILE_PATH     = ../common

LIBS         += -lm


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


INCLUDEPATH += ../../lib/include

QMAKE_CXXFLAGS_RELEASE  = -O0

message ("Host = " $${QMAKE_HOST.arch})

unix {
contains(QMAKE_HOST.arch, x86_64):{
    LIBS        += -L../../lib/linux64 -ludp
}

contains(QMAKE_HOST.arch, i686):{
    LIBS        += -L../../lib/linux32 -ludp
}
}

win32 {
    LIBS        += -L../../lib/win32 -ludp
#   QMAKE_POST_LINK += "copy ..\..\lib\win32\*.* bin  /y /q"
}
