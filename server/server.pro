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
        ../grading-assistant/gradingassistant.cpp \
        ../grading-assistant/databasemanager.cpp \
        ../grading-assistant/databasetable.cpp \
        ../grading-assistant/gadata/gaclass.cpp \
        ../grading-assistant/gadata/gaassignment.cpp \
        ../grading-assistant/gadata/gastudent.cpp \
        ../grading-assistant/gadata/gaannotation.cpp \
        ../grading-assistant/gadata/gaassignmentdata.cpp \
        ../grading-assistant/gadata/gaidentifiableobject.cpp \
        ../grading-assistant/gadata/garubric.cpp \
        ../grading-assistant/gadata/garubricrow.cpp \
        ../grading-assistant/gaoutputfile.cpp \

HEADERS += \
    githandler.h \
    ../grading-assistant/filemanager.h \
    ../grading-assistant/platform.h \
    ../grading-assistant/usersettings.h \
        ../grading-assistant/gradingassistant.h \
        ../grading-assistant/databasemanager.h \
        ../grading-assistant/databasetable.h \
        ../grading-assistant/gadata/gaclass.h \
        ../grading-assistant/gadata/gaassignment.h \
        ../grading-assistant/gadata/gastudent.h \
        ../grading-assistant/gadata/gaannotation.h \
        ../grading-assistant/gadata/gaassignmentdata.h \
        ../grading-assistant/gadata/gaidentifiableobject.h \
        ../grading-assistant/gadata/garubric.h \
        ../grading-assistant/gadata/garubricrow.h \
        ../grading-assistant/gaoutputfile.h \

!win32:LIBS += -l sqlite3

# These files must be downloaded as the amalgamation from SQLite and place in this folder.
# Be sure to add to PATH for sqlite3 usage
# https://sqlite.org/download.html
win32 {
    SOURCES += C:\sqlite\sqlite3.c
    HEADERS += C:\sqlite\sqlite3.h
}
