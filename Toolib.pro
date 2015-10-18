TARGET = Toolib
TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++11

QMAKE_CXXFLAGS_WARN_ON += -Wextra

CONFIG += debug_and_release
CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_dbg)
}

DESTDIR = $$PWD/lib

DEFINES += TOOLIB_LIBRARY

SOURCES += \
    src/ToolibDummy.cpp \
    src/filesys/impl_too/FileSys_too.cpp

HEADERS += \
    Toolib/ToolibDummy.h \
    ToolibDEF.h \
    Toolib/filesys/impl_too/FileSys_too.h
