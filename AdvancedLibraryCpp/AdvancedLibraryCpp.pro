#-------------------------------------------------
#
# Project created by QtCreator 2020-04-02T11:55:00
#
#-------------------------------------------------

QT -= gui

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
        binary_tree.h \
        concurrency/safe.h \
        delimiters.h \
        divisors.h \
        enum_factory.h \
        fenwick_tree.h \
        heap.h \
        math_operation.h \
        memory.h \
        segment_tree.h \
        tree.h \
        union_find.h \
        union_set.h

testmode {
    # ONLY TESTING UNIT
    QT       += testlib
    TARGET    = tests
    HEADERS  += test/testenum.h \
                test/testlockable.h \
                test/test_stream_delimiters.h \
                test/test_memory.h
    SOURCES  += test/testlockable.cpp \
                test/testenum.cpp \
                test/test_memory.cpp \
                test/test_stream_delimiters.cpp \
                test/tests.cpp
    TEMPLATE  = app
    CONFIG   += c++14 - Wall
}

libmode {
    #ONLY LIB
    TARGET    = AdvancedLibraryCpp
    TEMPLATE  = lib
    CONFIG   += staticlib
    CONFIG   += c++11 - Wall
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}
