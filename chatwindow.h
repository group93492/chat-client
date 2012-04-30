#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include "ChatClient.h"
#include "RegisterBot.h"

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = 0);
    ~ChatWindow();
	
private:
    Ui::ChatWindow *ui;
    ChatClient *m_client;

signals:
    void sendMessage(const QString &, const QString &);

private slots:
    void clientError(const QString &);
    void displayMessage(const QString &msgText);
    void clientAuthorized();
    void postMessage();
    void on_pushButton_2_clicked();
public slots:
    void connectToServer(QString username, QString password, QTcpSocket *socket);
};

#endif // CHATWINDOW_H
