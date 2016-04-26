TEMPLATE = app

QT += qml quick widgets
CONFIG += c++11

INCLUDEPATH += src\

SOURCES +=  src/main.cpp \
    src/dcsettings.cpp \
    src/dreamcraft.cpp \
    src/downloadprocess.cpp \
    src/downloadthread.cpp \
    src/dcsystemtrayicon.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    src/dreamcraft.h \
    src/dcsettings.h \
    src/downloadprocess.h \
    src/downloadthread.h \
    src/dcsystemtrayicon.h

RC_FILE=icon.rc
