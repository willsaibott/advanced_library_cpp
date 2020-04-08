#-------------------------------------------------
#
# Project created by QtCreator 2020-04-02T11:55:00
#
#-------------------------------------------------

QT       -= gui

TARGET = AdvancedLibraryCpp
TEMPLATE = lib
CONFIG += staticlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES +=

HEADERS += \
        binary_tree.h \
        delimiters.h \
        fenwick_tree.h \
        heap.h \
        math_operation.h \
        memory.h \
        segment_tree.h \
        tree.h \
        union_find.h \
        union_set.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
