#include <QtGui/QApplication>
#include "chatwindow.h"
#include "ChatAuthWindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChatWindow mainwindow;
    ChatAuthWindow authwindow;

    authwindow.exec();
    //Ok and Cancell buttons are temporary
    mainwindow.show();

    return a.exec();
}
