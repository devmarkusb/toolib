TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

QMAKE_CXXFLAGS_WARN_ON += -Wextra

INCLUDEPATH += \
    c:/projects/externlibs/gtest-1.7.0/include \
    c:/projects/externlibs/boost_1_56_0 \
    ../sdks/GSL/include \
    ..

LIBS += -L/projects/externlibs/gtest-1.7.0/ -lgtest

SOURCES += \
    c:/projects/externlibs/gtest-1.7.0/src/gtest_main.cc \
    Toolib/performanceTest.cpp \
    Toolib/assertTest.cpp \
    Toolib/vector/vectorTest.cpp \
    Toolib/math/numberTest.cpp \
    Toolib/math/roundTest.cpp \
    Toolib/class/IAbstractTest.cpp \
    Toolib/filesys/pathTest.cpp \
    ../src/filesys/path.cpp \
    Toolib/math/scaleTest.cpp \
    Toolib/math/floating_pointTest.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    ../Toolib/class/IAbstract.h \
    ../Toolib/math/scale.h \
    ../Toolib/math/floating_point.h \
    ../Toolib/std/std_extensions.h

