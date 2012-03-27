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
    RegisterBot *m_regBot;

signals:
    void sendMessage(const QString &, const QString &);
private slots:
    void connectToServer();
    void clientError(const QString &);
    void displayMessage(const QString &msgText);
    void channelListUpdate(QStringList &channels);
    void clientAuthorized();
    void postMessage();
    void on_disconnectButton_clicked();
    void requestRegistration();
    void registrationCompleted(bool regResult, QString &denialReason);
    void registrationError(QString &errorMsg);
};

#endif // CHATWINDOW_H
