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
    QStringList channelList;
    void sendMessageToServer(ChatMessageBody *msgBody) const;
    void processMessage(const ChannelMessage *msg);
    void processMessage(const DisconnectMessage *msg);
    void processMessage(const RegistrationAnswer *msg);
    void processMessage(const ChannelListMessage *msg);
    void processMessage(const ChannelJoinResult *msg);
    void processMessage(const ChannelSystemMessage *msg);
public:
    explicit ChatClient(QObject *parent = 0);
    void setUserInfo(const QString &un, const QString &pass);
    bool start(QTcpSocket *socket);
    void stop();

signals:
    void errorOccured(const QString &);
    void messageToDisplay(const QString &);
    void clientAuthorized();
    void displayChannelList(QStringList &);
    void channelJoin(QString channelname, bool result);
    void channelSystemMsg(QString channel, QString msg);

private slots:
    void clientConnected() const;
    void clientGotNewMessage();
    void socketError(const QAbstractSocket::SocketError &error);

public slots:
    void sendChannelMessage(const QString &rcvr, const QString &body) const;
    void sendDisconnectMessage() const;
    void allChannelsRequest();
    void joinChannelRequest(QString channelname);
    void leaveChannel(QString channelname);

};

#endif // CHATCLIENT_H
