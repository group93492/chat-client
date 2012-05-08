#include <QtGui/QApplication>
#include "chatwindow.h"
#include "ChatAuthWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChatWindow mainwindow;
    ChatAuthWindow authwindow;
    QObject::connect(&authwindow, SIGNAL(authorized(QString,QString,QTcpSocket*)), &mainwindow, SLOT(connectToServer(QString,QString,QTcpSocket*)));
    authwindow.show();
    return a.exec();
}
