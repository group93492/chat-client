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
    ../chat-server-polygon/ChatMessageSerializer.cpp \
    ../chat-server-polygon/ChatMessages.cpp \
    ChatClient.cpp

HEADERS  += chatwindow.h \
    ../chat-server-polygon/ChatMessageSerializer.h \
    ../chat-server-polygon/ChatMessages.h \
    ChatClient.h

FORMS    += chatwindow.ui
