#ifndef REGISTERBOT_H
#define REGISTERBOT_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include "ChatMessages.h"

class RegisterBot : public QObject
{
    Q_OBJECT
private:
    QTcpSocket *m_socket;
    quint16 m_nextBlockSize;
    QString m_host;
    quint32 m_port;
    QString m_username;
    QString m_password;
    void sendMessageToServer(ChatMessageBody *msgBody) const;
    void processMessage(RegistrationAnswer *msg);
public:
    explicit RegisterBot(QObject *parent = 0);
    RegisterBot(QString &host, quint32 port, QString &username, QString &password);
    ~RegisterBot();
    void closeConnection();
signals:
    void errorOccured(QString &errorMsg);
    void registrationCompleted(bool registrationResult, QString &denialReason);
private slots:
    void botConnected();
    void botGotMessage();
    void socketError(QAbstractSocket::SocketError error);

};

#endif // REGISTERBOT_H
