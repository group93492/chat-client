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
    ChatClient *client;

signals:
    void sendMessage(QString &, QString &);

private slots:
    void connectToServer();
    void clientError(QString &);
    void displayMessage(QString &);
    void clientAuthorized();
    void postMessage();
    void on_connectPropsGB_clicked();
};

#endif // CHATWINDOW_H
