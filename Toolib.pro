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
    src/err.cpp

HEADERS += \
    Toolib/err.h
