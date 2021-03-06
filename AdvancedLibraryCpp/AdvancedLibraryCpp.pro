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
        classic/sequences/missing_element.h \
        classic/sequences/subarray.h \
        classic/sequences/subset.h \
        concurrency/message_queue.h \
        concurrency/safe.h \
        concurrency/semaphore.h \
        concurrency/thread.h \
        concurrency/timer.h \
        structures/binary_tree.h \
        structures/cache.h \
        structures/command.h \
        structures/fenwick_tree.h \
        structures/heap.h \
        structures/segment_tree.h \
        structures/tree.h \
        structures/union_find.h \
        structures/union_set.h \
        test/test_read_csv.h \
        tools/delimiters.h \
        tools/divisors.h \
        tools/enum_factory.h \
        tools/math_operation.h \
        tools/memory.h \
        tools/random.h \
        tools/read_csv.h \
        tools/timestamp.h \
        tools/types.h

CONFIG+=testmode
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
                test/simple_protocol_moc.h \
                test/simple_worker_moc.h \
                test/test_binary_tree.h \
                test/test_avl_tree.h \
                test/test_command.h \
                test/test_fenwick_tree.h \
                test/test_heap.h \
                test/test_lru_cache.h \
                test/test_message_queue.h \
                test/test_segment_tree.h \
                test/test_semaphore.h \
                test/test_timer.h \
                test/test_tree.h \
                test/test_union_find.h \
                test/test_union_set.h \
                test/test_wrapper_thread.h \
                test/test_math.h \
                test/test_timestamp.h \
                test/test_lockable.h
    SOURCES  += \
                test/test_memory.cpp \
                test/test_stream_delimiters.cpp \
                test/test_enum.cpp \
                test/test_random.cpp \
                test/test_lockable.cpp \
                test/test_missing_elements.cpp \
                test/test_subarray.cpp \
                test/test_subset.cpp \
                test/test_binary_tree.cpp \
                test/test_command.cpp \
                test/test_fenwick_tree.cpp \
                test/test_heap.cpp \
                test/test_lru_cache.cpp \
                test/test_message_queue.cpp \
                test/test_segment_tree.cpp \
                test/test_semaphore.cpp \
                test/test_timer.cpp \
                test/test_tree.cpp \
                test/test_union_find.cpp \
                test/test_union_set.cpp \
                test/test_wrapper_thread.cpp \
                test/test_avl_tree.cpp \
                test/test_math.cpp \
                test/test_timestamp.cpp \
                test/tests.cpp

    TEMPLATE  = app
    CONFIG   += c++17 - Wall
    LIBS     += -lstdc++fs
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

INCLUDEPATH += source \
               ../lib/source

include(qmake-target-platform.pri)
include(qmake-destination-path.pri)

DISTFILES += \
  ../config \
  qmake-destination-path.pri

DESTDIR     = $$PWD/binaries/$$DESTINATION_PATH
OBJECTS_DIR = $$PWD/build/$$DESTINATION_PATH/.obj
MOC_DIR     = $$PWD/build/$$DESTINATION_PATH/.moc
RCC_DIR     = $$PWD/build/$$DESTINATION_PATH/.qrc
UI_DIR      = $$PWD/build/$$DESTINATION_PATH/.ui
!build_pass:message(test output dir: $${DESTDIR})

win32:VERSION = 0.1.0.0 # major.minor.patch.build
else:VERSION  = 0.1.0   # major.minor.patch

# --coverage option is synonym for: -fprofile-arcs -ftest-coverage -lgcov
QMAKE_CXXFLAGS += --coverage -std=c++17
QMAKE_LFLAGS += --coverage

QMAKE_POST_LINK = rm -f build/$$DESTINATION_PATH/.obj/*gcda
!build_pass:message(cleaning: rm -f "$$OBJECTS_DIR/*gcda")

SOURCES += \
    test/test_read_csv.cpp

RESOURCES += \
    tests.qrc

