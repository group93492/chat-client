#-------------------------------------------------
#
# Project created by QtCreator 2012-02-20T22:18:32
#
#-------------------------------------------------

QT       += core gui network

TARGET = chat-client
TEMPLATE = app


SOURCES +=  main.cpp\
            chatwindow.cpp \
            ChatClient.cpp \
            ChatMessages.cpp

HEADERS  += chatwindow.h \
            ChatClient.h \
            ChatMessages.h

FORMS    += chatwindow.ui
