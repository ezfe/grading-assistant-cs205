#-------------------------------------------------
#
# Project created by QtCreator 2017-03-30T13:43:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GAUI
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        basescreen.cpp \
        ../grading-assistant/gradingassistant.cpp \
        ../grading-assistant/databasemanager.cpp \
        ../grading-assistant/databasetable.cpp \
        ../grading-assistant/filemanager.cpp \
        ../grading-assistant/usersettings.cpp \
        ../grading-assistant/gadata/gaclass.cpp \
        ../grading-assistant/gadata/gaassignment.cpp \
        ../grading-assistant/gadata/gastudent.cpp \
        ../grading-assistant/gadata/gaannotation.cpp \
        ../grading-assistant/gadata/gaassignmentdata.cpp \
        ../grading-assistant/gadata/gaidentifiableobject.cpp \
        ../grading-assistant/gadata/garubric.cpp \
        ../grading-assistant/gadata/garubricrow.cpp \
        ../grading-assistant/gaoutputfile.cpp \
    rubricdialog.cpp \
    gradingsession.cpp \
    labstabwidget.cpp \
    setupsessiondialog.cpp \
    flagdialog.cpp \
    gradingdialog.cpp \
    addstudentdialog.cpp \
    addassignmentdialog.cpp

HEADERS  += basescreen.h \
        ../grading-assistant/gradingassistant.h \
        ../grading-assistant/databasemanager.h \
        ../grading-assistant/databasetable.h \
        ../grading-assistant/filemanager.h \
        ../grading-assistant/platform.h \
        ../grading-assistant/usersettings.h \
        ../grading-assistant/gadata/gaclass.h \
        ../grading-assistant/gadata/gaassignment.h \
        ../grading-assistant/gadata/gastudent.h \
        ../grading-assistant/gadata/gaannotation.h \
        ../grading-assistant/gadata/gaassignmentdata.h \
        ../grading-assistant/gadata/gaidentifiableobject.h \
        ../grading-assistant/gadata/garubric.h \
        ../grading-assistant/gadata/garubricrow.h \
        ../grading-assistant/gaoutputfile.h \
    rubricdialog.h \
    gradingsession.h \
    labstabwidget.h \
    setupsessiondialog.h \
    flagdialog.h \
    gradingdialog.h \
    addstudentdialog.h \
    addassignmentdialog.h

FORMS    += basescreen.ui \
    rubricdialog.ui \
    gradingsession.ui \
    labstabwidget.ui \
    setupsessiondialog.ui \
    flagdialog.ui \
    gradingdialog.ui \
    addstudentdialog.ui \
    addassignmentdialog.ui

LIBS     += -l sqlite3
