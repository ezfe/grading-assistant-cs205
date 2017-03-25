TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    student.cpp \
    class.cpp \
    annotation.cpp \
    assignment.cpp \
    gradingassistant.cpp \
    usersettings.cpp \
    filemanager.cpp

HEADERS += \
    student.h \
    class.h \
    annotation.h \
    assignment.h \
    gradingassistant.h \
    usersettings.h \    
    filemanager.h
