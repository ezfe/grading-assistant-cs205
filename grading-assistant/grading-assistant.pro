TEMPLATE = app

CONFIG += console c++11
CONFIG -= app_bundle

QT += core
QT -= gui

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp \
    gradingassistant.cpp \
    usersettings.cpp \
    filemanager.cpp \
    databasemanager.cpp \
    gadata/gaannotation.cpp \
    gadata/gaidentifiableobject.cpp \
    gadata/gaclass.cpp \
    gadata/gaassignment.cpp \
    gadata/gastudent.cpp \
    gadata/gaassignmentdata.cpp \
    gadata/garubric.cpp \
    gadata/garubricrow.cpp \
    databasetable.cpp

HEADERS += \
    gradingassistant.h \
    usersettings.h \    
    filemanager.h \
    databasemanager.h \
    platform.h \
    gadata/gaannotation.h \
    gadata/gaidentifiableobject.h \
    gadata/gaclass.h \
    gadata/gaassignment.h \
    gadata/gastudent.h \
    gadata/gaassignmentdata.h \
    gadata/garubric.h \
    gadata/garubricrow.h \
    databasetable.h

LIBS += -l sqlite3
