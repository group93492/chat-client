#-------------------------------------------------
#
# Project created by QtCreator 2012-02-20T22:18:32
#
#-------------------------------------------------

QT       += core gui network

TARGET = chat-client-polygon
TEMPLATE = app


SOURCES += main.cpp\
        chatwindow.cpp \
    ChatMessageSerializer.cpp \
    ChatMessages.cpp \
    ChatClient.cpp

HEADERS  += chatwindow.h \
    ChatMessageSerializer.h \
    ChatMessages.h \
    ChatClient.h

FORMS    += chatwindow.ui
