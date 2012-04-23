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

bool ChatClient::start(const QString &host, const quint16 &port)
{
    if (!m_userdataAssigned)
        return false;
    m_tcpSocket = new QTcpSocket(this);
    connect(m_tcpSocket, SIGNAL(connected()), SLOT(clientConnected()));
    connect(m_tcpSocket, SIGNAL(readyRead()), SLOT(clientGotNewMessage()));
    connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(socketError(QAbstractSocket::SocketError)));
    m_tcpSocket->connectToHost(host, port);
    return true;
}

void ChatClient::stop()
{

}

void ChatClient::clientConnected() const
{
    //we were connected to server
    //now we need to authorize
    AuthorizationRequest *msg = new AuthorizationRequest();
    msg->username = this->m_username;
    msg->password = this->m_password;
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
        if(m_nextBlockSize == 0)
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
        case cmtAuthorizationAnswer:
            {
                AuthorizationAnswer *msg = new AuthorizationAnswer(input);
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
        default:
            {
                qDebug() << "Client received unknown-typed message" << msgType;
                /*m_nextBlockSize = 0;
                return;*/
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
//    QString message = "Disconnect from server";
    delete msg;
}

void ChatClient::allChannelsRequest()
{
    ChannelListRequest *list = new ChannelListRequest();
    list->listType = ChannelListRequest::listOfAll;
    list->nick = m_username;
    sendMessageToServer(list);
}

void ChatClient::sendMessageToServer(ChatMessageBody *msgBody) const
{
    QByteArray arrBlock;
    QDataStream output(&arrBlock, QIODevice::WriteOnly);
    output.setVersion(QDataStream::Qt_4_7);
    output << quint16(0);
//    ChatMessageSerializer::packMessage(output, msgBody);
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
    QString message = QString("%1:: %2 - %3")
                        .arg(msg->receiver)
                        .arg(msg->sender)
                        .arg(msg->messageText);
    emit messageToDisplay(message);
}

void ChatClient::processMessage(const AuthorizationAnswer *msg)
{
    qDebug() << "Processing authorization answer:" << msg->authorizationResult;
    if (m_authorized)
        qDebug() << "Allready authorizated, don't need to process that";
    if (msg->authorizationResult)
    {
        m_authorized = true;
        //QString disp = "You passed authorization on server: " + tcpSocket->peerAddress().toString();
        //emit messageToDisplay(disp);
        emit clientAuthorized();
    }
    else
    {
        QString err = "Authorization problem: " + msg->denialReason;
        emit errorOccured(err);
    }
}

void ChatClient::processMessage(const DisconnectMessage *msg)
{
    QString message;
    message = msg->sender + " has left from server";
    qDebug() << "Processing disconnect message from:" << msg->sender;
    emit messageToDisplay(message);
}

void ChatClient::processMessage(const ChannelListMessage *msg)
{
    channelList = msg->channelList.keys();
    emit displayChannelList(channelList);
}

