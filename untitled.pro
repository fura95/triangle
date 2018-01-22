#-------------------------------------------------
#
# Project created by QtCreator 2015-03-15T16:49:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app

QMAKE_CXXFLAGS += -openmp

#QMAKE_LIBS+=-static -lgomp -lpthread
#QMAKE_CXXFLAGS+=-fopenmp

win32:RC_FILE = icon.rc


SOURCES += main.cpp\
        mainwindow.cpp \
    triangulation.cpp \
    triangulationwithcolor.cpp

HEADERS  += mainwindow.h \
    triangulation.h \
    triangulationwithcolor.h

FORMS    += mainwindow.ui


unix|win32: LIBS += -L$$PWD/../../Desktop/fadeRelease_v1.20/fadeRelease/x64/ -lfade25D_vc12_Release

INCLUDEPATH += $$PWD/../../Desktop/fadeRelease_v1.20/fadeRelease/include_fade25d
DEPENDPATH += $$PWD/../../Desktop/fadeRelease_v1.20/fadeRelease/include_fade25d

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../Desktop/fadeRelease_v1.20/fadeRelease/x64/fade25D_vc12_Release.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../../Desktop/fadeRelease_v1.20/fadeRelease/x64/libfade25D_vc12_Release.a

unix|win32: LIBS += -L$$PWD/../../Desktop/fadeRelease_v1.20/fadeRelease/x64/ -llibgmp-10

INCLUDEPATH += $$PWD/../../Desktop/fadeRelease_v1.20/fadeRelease/include_fade25d
DEPENDPATH += $$PWD/../../Desktop/fadeRelease_v1.20/fadeRelease/include_fade25d

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../Desktop/fadeRelease_v1.20/fadeRelease/x64/libgmp-10.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../../Desktop/fadeRelease_v1.20/fadeRelease/x64/liblibgmp-10.a
