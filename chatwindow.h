#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include "ChatClient.h"
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
    void connectToServer();
    void clientError(const QString &);
    void displayMessage(const QString &);
    void clientAuthorized();
    void postMessage();
};

#endif // CHATWINDOW_H
