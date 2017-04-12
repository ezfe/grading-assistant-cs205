TEMPLATE = app

CONFIG += console c++11
CONFIG -= app_bundle

QT += core
QT -= gui

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp \
    githandler.cpp

HEADERS += \
    githandler.h


LIBS += -l sqlite3
