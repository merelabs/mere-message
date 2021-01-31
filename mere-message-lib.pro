QT -= gui

TARGET = mere-message
TEMPLATE = lib

CONFIG += c++11
CONFIG += shared

DEFINES += MERE_MESSAGE_LIB

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/client.cpp \
    src/command.cpp \
    src/config.cpp \
    src/error.cpp \
    src/id.cpp \
    src/masker.cpp \
    src/message.cpp \
    src/messenger.cpp \
    src/notifier.cpp \
    src/receiver.cpp \
    src/sender.cpp \
    src/server.cpp \
    src/space.cpp \
    src/uri.cpp \
    src/utils.cpp

HEADERS += \
    src/command.h \
    src/config.h \
    src/error.h \
    src/global.h \
    src/client.h \
    src/id.h \
    src/masker.h \
    src/message.h \
    src/messenger.h \
    src/notifier.h \
    src/receiver.h \
    src/sender.h \
    src/server.h \
    src/space.h \
    src/types.h \
    src/uri.h \
    src/utils.h

LIBS += -lrt

#
# Install
#
unix {
    target.path = /usr/local/lib
    INSTALLS += target

    INSTALL_PREFIX = /usr/local/include/mere/message
    for(header, HEADERS) {
        sdir = $${dirname(header)}
        sdir = $$replace(sdir, "src", "")
        path = $${INSTALL_PREFIX}$${sdir}

        eval(headers_$${path}.files += $$header)
        eval(headers_$${path}.path = $$path)
        eval(INSTALLS *= headers_$${path})
    }
}
#!isEmpty(target.path): INSTALLS += target
