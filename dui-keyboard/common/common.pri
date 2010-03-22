COMMON_DIR = ./common

INSTALL_HEADERS += \
    $$COMMON_DIR/vkbdatakey.h \
    $$COMMON_DIR/keyevent.h \

HEADERS += \
    $$COMMON_DIR/duikeyboardcommon.h \
    $$COMMON_DIR/keyboarddata.h \
    $$COMMON_DIR/keyevent.h \
    $$COMMON_DIR/layoutdata.h \
    $$COMMON_DIR/layoutsmanager.h \
    $$COMMON_DIR/limitedtimer.h \
    $$COMMON_DIR/toolbardata.h \
    $$COMMON_DIR/toolbarbutton.h \
    $$COMMON_DIR/toolbarmanager.h \
    $$COMMON_DIR/vkbdatakey.h \
    $$COMMON_DIR/duixkb_p.h \
    $$COMMON_DIR/duixkb.h \
    $$COMMON_DIR/duihardwarekeyboard.h \
    $$COMMON_DIR/hwkbcharloops.h \
    $$COMMON_DIR/hwkbcharloopsmanager.h \

SOURCES += \
    $$COMMON_DIR/keyboarddata.cpp\
    $$COMMON_DIR/keyevent.cpp\
    $$COMMON_DIR/layoutdata.cpp\
    $$COMMON_DIR/layoutsmanager.cpp\
    $$COMMON_DIR/limitedtimer.cpp\
    $$COMMON_DIR/toolbardata.cpp\
    $$COMMON_DIR/toolbarbutton.cpp \
    $$COMMON_DIR/toolbarmanager.cpp\
    $$COMMON_DIR/vkbdatakey.cpp\
    $$COMMON_DIR/duixkb.cpp \
    $$COMMON_DIR/duihardwarekeyboard.cpp \
    $$COMMON_DIR/hwkbcharloops.cpp \
    $$COMMON_DIR/hwkbcharloopsmanager.cpp \

INCLUDEPATH += $$COMMON_DIR
DEPENDPATH += $$COMMON_DIR

