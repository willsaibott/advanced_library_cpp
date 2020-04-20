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
        classic/sequences/missing_element.h \
        classic/sequences/subarray.h \
        classic/sequences/subset.h \
        concurrency/safe.h \
        delimiters.h \
        divisors.h \
        enum_factory.h \
        fenwick_tree.h \
        heap.h \
        math_operation.h \
        memory.h \
        random.h \
        segment_tree.h \
        structures/binary_tree.h \
        structures/fenwick_tree.h \
        structures/heap.h \
        structures/segment_tree.h \
        structures/tree.h \
        structures/union_find.h \
        structures/union_set.h \
        tools/delimiters.h \
        tools/divisors.h \
        tools/enum_factory.h \
        tools/math_operation.h \
        tools/memory.h \
        tools/random.h \
        tools/types.h \
        tree.h \
        types.h \
        union_find.h \
        union_set.h

testmode {
    # ONLY TESTING UNIT
    QT       += testlib
    TARGET    = tests
    HEADERS  += \
                test/test_stream_delimiters.h \
                test/test_memory.h \
                test/test_enum.h \
                test/test_random.h \
                test/test_missing_elements.h \
                test/test_subarray.h \
                test/test_subset.h \
                test/test_lockable.h
    SOURCES  += \
                test/test_memory.cpp \
                test/test_stream_delimiters.cpp \
                test/tests.cpp \
                test/test_enum.cpp \
                test/test_random.cpp \
                test/test_lockable.cpp \
                test/test_missing_elements.cpp \
                test/test_subarray.cpp \
                test/test_subset.cpp
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
