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
    gaannotation.cpp \
    gaidentifiableobject.cpp \
    gaclass.cpp \
    gaassignment.cpp \
    gastudent.cpp \
    gaassignmentdata.cpp \
    garubric.cpp \
    garubricrow.cpp \
    databasetable.cpp

HEADERS += \
    gradingassistant.h \
    usersettings.h \    
    filemanager.h \
    databasemanager.h \
    platform.h \
    gaannotation.h \
    gaidentifiableobject.h \
    gaclass.h \
    gaassignment.h \
    gastudent.h \
    gaassignmentdata.h \
    garubric.h \
    garubricrow.h \
    databasetable.h

LIBS += -l sqlite3
