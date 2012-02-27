#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "ChatMessages.h"

class ChatClient : public QObject
{
    Q_OBJECT
private:
    QTcpSocket *m_tcpSocket;
    quint16 m_nextBlockSize;
    QString m_username;
    QString m_password;   //password stored in string, kekeke
    bool m_userdataAssigned;
    bool m_authorized;
    void sendMessageToServer(ChatMessageBody *msgBody);
    void processMessage(ChannelMessage *msg);
    void processMessage(AuthorizationAnswer *msg);
public:
    explicit ChatClient(QObject *parent = 0);
    void setUserInfo(QString &un, QString &pass);
    bool start(QString &host, quint16 port);
signals:
    void errorOccured(QString &);
    void messageToDisplay(QString &);
    void clientAuthorized();
private slots:
    void clientConnected();
    void clientGotNewMessage();
    void socketError(QAbstractSocket::SocketError error);
public slots:
    void sendChannelMessage(QString& rcvr, QString &body);
};

#endif // CHATCLIENT_H
