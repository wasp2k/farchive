# -------------------------------------------------
# Project created by QtCreator 2009-08-22T09:55:52
# -------------------------------------------------
QT -= gui
TARGET = farchivetest
CONFIG += console x86
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += farchivetest.cpp


INCLUDEPATH += ../src
LIBS += -L../ -lfarchive
