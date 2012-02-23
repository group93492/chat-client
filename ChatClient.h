#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "ChatMessageSerializer.h"
#include "ChatMessages.h"

class ChatClient : public QObject
{
    Q_OBJECT
private:
    QTcpSocket *tcpSocket;
    quint16 nextBlockSize;
    QString username;
    QString password;   //password stored in string, kekeke
    bool userdataAssigned;
    bool authorized;
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
