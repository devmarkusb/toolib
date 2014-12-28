TARGET = Toolib_Qt
TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++11

QMAKE_CXXFLAGS_WARN_ON += -Wextra

CONFIG += debug_and_release
CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_dbg)
}

DESTDIR = $$PWD/lib

DEFINES += TOOLIB_QT_LIBRARY

QT += widgets

INCLUDEPATH += \
    /projects/externlibs/boost_1_56_0 \

SOURCES += \
    src/filesys/impl_Qt/FileSys_Qt.cpp \
    src/messagebox/impl_Qt/MessageBox_Qt.cpp

HEADERS += \
    Toolib_QtDEF.h \
    Toolib/filesys/impl_Qt/FileSys_Qt.h \
    Toolib/messagebox/impl_Qt/MessageBox_Qt.h \
    Toolib/filesys.h \
    Toolib/imessagebox.h \
    Toolib/iprogramsettings.h
