
TARGET = console
CONFIG  += console

SOURCES      += main.cpp
#SOURCES      += apds9960.cpp
#HEADERS      += apds9960.h

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
LIBS        += -L..\udplib\bin -ludp
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



INCLUDEPATH  += ../../udplib
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
INCLUDEPATH += ../../../../../git/emulator_v0/lib/include
LIBS        += -L../../udplib/bin -ludp

QMAKE_CXXFLAGS_RELEASE  = -O0

message ("Host = " $${QMAKE_HOST.arch})


message ("Host = " $${QMAKE_HOST.arch})

unix {
contains(QMAKE_HOST.arch, x86_64):{
    LIBS        += -L../../udplib/linux64 -ludp
}

contains(QMAKE_HOST.arch, i686):{
    LIBS        += -L../../udplib/linux32 -ludp
}
}

win32 {
    LIBS        += -L../../udplib/win32 -ludp
#   QMAKE_POST_LINK += "copy ..\..\lib\win32\*.* bin  /y /q"
}

unix {
    DEFINES += LINUX_WAY
}


