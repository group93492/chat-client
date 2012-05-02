#include "ChatClient.h"
#include <QHostAddress>

ChatClient::ChatClient(QObject *parent) :
    QObject(parent),
    m_nextBlockSize(0),
    m_userdataAssigned(false),
    m_authorized(false)
{
}

void ChatClient::setUserInfo(const QString &un, const QString &pass)
{
    m_username = un;
    m_password = pass;
    m_userdataAssigned = !((m_username.isEmpty()) || (m_password.isEmpty()));
}

QString ChatClient::username()
{
    return m_username;
}

bool ChatClient::start(QTcpSocket *socket)
{
    if (!m_userdataAssigned)
        return false;
    m_tcpSocket = socket;
    connect(m_tcpSocket, SIGNAL(readyRead()), SLOT(clientGotNewMessage()));
    connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(socketError(QAbstractSocket::SocketError)));
    //request list of all channels from server
    ChannelListRequest *msg = new ChannelListRequest();
    msg->listType = ChannelListRequest::listOfAll;
    msg->nick = m_username;
    sendMessageToServer(msg);
    //and also request list of channels joined by client
    msg->listType = ChannelListRequest::listOfJoined;
    sendMessageToServer(msg);
    delete msg;
    return true;
}

void ChatClient::shutdown()
{
    DisconnectMessage *msg = new DisconnectMessage();
    msg->sender = username();
    sendMessageToServer(msg);
    delete msg;
}

void ChatClient::clientGotNewMessage()
{
    qDebug() << "Client received new message";
    QTcpSocket *socket = (QTcpSocket*)sender();
    QDataStream input(socket);
    input.setVersion(QDataStream::Qt_4_7);
    while (true)
    {
        if(!m_nextBlockSize)
        {
            if(socket->bytesAvailable() < sizeof(quint16))
                break;
            input >> m_nextBlockSize;
        }
        if(socket->bytesAvailable() < m_nextBlockSize)
            break;
        //message in in <input>, unpack it
        ChatMessageHeader *header = new ChatMessageHeader(input);
        ChatMessageType msgType = (ChatMessageType) header->messageType;
        delete header;
        switch (msgType)
        {
        case cmtChannelMessage:
            {
                ChannelMessage *msg = new ChannelMessage(input);
                processMessage(msg);
                delete msg;
                break;
            }
        case cmtDisconnectMessage:
            {
                DisconnectMessage *msg = new DisconnectMessage(input);
                processMessage(msg);
                delete msg;
                break;
            }
        case cmtChannelListMessage:
            {
                ChannelListMessage *msg = new ChannelListMessage(input);
                processMessage(msg);
                delete msg;
                break;
            }
        case cmtChannelJoinResult:
            {
                ChannelJoinResult *msg = new ChannelJoinResult(input);
                processMessage(msg);
                delete msg;
                break;
            }
        case cmtChannelSystemMessage:
            {
                ChannelSystemMessage *msg = new ChannelSystemMessage(input);
                processMessage(msg);
                delete msg;
                break;
            }
        case cmtChannelUserList:
            {
                ChannelUserList *msg = new ChannelUserList(input);
                processMessage(msg);
                delete msg;
                break;
            }
        default:
            {
                qDebug() << "Client received unknown-typed message" << msgType;
                break;
            }
        }
        m_nextBlockSize = 0;
    }
}

void ChatClient::socketError(const QAbstractSocket::SocketError &error)
{
    QString strError =
            (error == QAbstractSocket::HostNotFoundError ?
                 "The host was not found." :
                 error == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     error == QAbstractSocket::ConnectionRefusedError ?
                         "The connect was refused." :
                         QString(m_tcpSocket->errorString()));
    emit errorOccured(strError);
    qDebug() << strError;
}

void ChatClient::sendChannelMessage(const QString &rcvr, const QString &body) const
{
    ChannelMessage *msg = new ChannelMessage();
    msg->sender = m_username;
    msg->receiver = rcvr;
    msg->messageText = body;
    sendMessageToServer(msg);
    delete msg;
}

void ChatClient::sendDisconnectMessage() const
{
    DisconnectMessage *msg = new DisconnectMessage();
    msg->sender = m_username;
    sendMessageToServer(msg);
    m_tcpSocket->close();
    delete msg;
}

void ChatClient::allChannelsRequest()
{
    ChannelListRequest *list = new ChannelListRequest();
    list->listType = ChannelListRequest::listOfAll;
    list->nick = m_username;
    sendMessageToServer(list);
    delete list;
}

void ChatClient::joinChannelRequest(QString channelname)
{
    ChannelJoinRequest *request = new ChannelJoinRequest();
    request->channelName = channelname;
    request->nick = m_username;
    sendMessageToServer(request);
    delete request;
}

void ChatClient::leaveChannel(QString channelname)
{
    ChannelLeaveMessage *msg = new ChannelLeaveMessage();
    msg->channelName = channelname;
    msg->nick = m_username;
    sendMessageToServer(msg);
    delete msg;
}

void ChatClient::sendMessageToServer(ChatMessageBody *msgBody) const
{
    QByteArray arrBlock;
    QDataStream output(&arrBlock, QIODevice::WriteOnly);
    output.setVersion(QDataStream::Qt_4_7);
    output << quint16(0);
    ChatMessageHeader *header = new ChatMessageHeader(msgBody);
    header->pack(output);
    msgBody->pack(output);
    delete header;
    output.device()->seek(0);
    output << quint16(arrBlock.size() - sizeof(quint16));
    m_tcpSocket->write(arrBlock);
}

void ChatClient::processMessage(const ChannelMessage *msg)
{
    qDebug() << "Processing channel message:" << msg->sender << msg->receiver << msg->messageText;
    emit channelMsg(msg->receiver, msg->sender, msg->messageText);
}

void ChatClient::processMessage(const DisconnectMessage *msg)
{
    QString message;
    message = msg->sender + " has left from server";
    qDebug() << "Processing disconnect message from:" << msg->sender;
    //emit messageToDisplay(message);
}

void ChatClient::processMessage(const ChannelListMessage *msg)
{
    ChatClient::ChannelListType type = ChatClient::ChannelListType(msg->listType);
    emit displayChannelList(msg->channelList, type);
}

void ChatClient::processMessage(const ChannelJoinResult *msg)
{
    QString channelname = msg->channelName;
    bool result = msg->result;
    emit channelJoin(channelname, result);
}

void ChatClient::processMessage(const ChannelSystemMessage *msg)
{
    QString channelname = msg->channelName;
    QString text = msg->message;
    emit channelSystemMsg(channelname, text);
}

void ChatClient::processMessage(const ChannelUserList *msg)
{
    QStringList list = msg->userList;
    QString channel = msg->channelName;
    emit userList(channel, list);
}

