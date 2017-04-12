TEMPLATE = app

CONFIG += console c++11
CONFIG -= app_bundle

QT += core
QT -= gui

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp \
    githandler.cpp \
    ../grading-assistant/filemanager.cpp \
    ../grading-assistant/usersettings.cpp \

HEADERS += \
    githandler.h \
    ../grading-assistant/filemanager.h \
    ../grading-assistant/platform.h \
    ../grading-assistant/usersettings.h

LIBS += -l sqlite3

#include "gadata/gaclass.h"
#include "gadata/gaassignment.h"
#include "gadata/gastudent.h"
#include "gadata/gaassignmentdata.h"
