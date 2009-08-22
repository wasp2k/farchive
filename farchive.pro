# -------------------------------------------------
# Project created by QtCreator 2009-07-26T10:10:22
# -------------------------------------------------
QT -= core \
    gui
CONFIG += staticlib
TARGET = farchive
CONFIG -= app_bundle
OBJECTS_DIR = objs
TEMPLATE = lib
SOURCES += src/ffile.cpp \
    src/ferror.cpp \
    src/fdbg.cpp \
    src/farchive.cpp \
    src/fobject.cpp
HEADERS += src/ffile.h \
    src/ferror.h \
    src/fdbg.h \
    src/farchive.h \
    src/fobject.h
