TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt

QT += core

LIBS += -l sqlite3

SOURCES += main.cpp

### add the following lines

# if you are having compiler complaints about pthread, uncomment the following
LIBS += -pthread

SOURCES += ../gtest/googletest/src/gtest-all.cc

INCLUDEPATH +=  ../gtest                                   \
                ../gtest/googletest                        \
                ../gtest/googletest/include                \
                ../gtest/googletest/include/gtest



SOURCES += ../grading-assistant/gradingassistant.cpp \
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
        ../grading-assistant/gaoutputfile.cpp


HEADERS  += ../grading-assistant/gradingassistant.h \
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
        ../grading-assistant/gaoutputfile.h

