#-------------------------------------------------
#
# Project created by QtCreator 2017-02-19T15:11:56
#
#-------------------------------------------------

QT += core gui widgets

TARGET = yanma_code_annotation_tool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    imageview.cpp \
    previewimageview.cpp \
    annotationdata.cpp \
    labelvalidator.cpp \
    labels.cpp

HEADERS  += mainwindow.h \
    imageview.h \
    previewimageview.h \
    annotationdata.h \
    labelvalidator.h \
    labels.h

FORMS    += mainwindow.ui

RESOURCES += resource.qrc

copydata.commands = cp $$PWD/labels.txt $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

