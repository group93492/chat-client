#include "ChatClient.h"
#include <QHostAddress>

ChatClient::ChatClient(QObject *parent) :
    QObject(parent),
    nextBlockSize(0),
    userdataAssigned(false),
    authorized(false)
{    
}

void ChatClient::setUserInfo(QString &un, QString &pass)
{
    username = un;
    password = pass;
    userdataAssigned = !((username.isEmpty()) || (password.isEmpty()));
}

bool ChatClient::start(QString &host, quint16 port)
{
    if (!userdataAssigned)
        return false;
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(connected()), SLOT(clientConnected()));
    connect(tcpSocket, SIGNAL(readyRead()), SLOT(clientGotNewMessage()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(socketError(QAbstractSocket::SocketError)));
    tcpSocket->connectToHost(host, port);
    return true;
}

void ChatClient::clientConnected()
{
    //we were connected to server
    //now we need to authorize
    AuthorizationRequest *msg = new AuthorizationRequest();
    msg->username = this->username;
    msg->password = this->password;
    sendMessageToServer(msg);
    delete msg;
}

void ChatClient::clientGotNewMessage()
{
    qDebug() << "Cleint received new message";
    QTcpSocket *socket = (QTcpSocket*)sender();
    QDataStream input(socket);
    input.setVersion(QDataStream::Qt_4_7);
    while (true)
    {
        if(!nextBlockSize)
        {
            if(socket->bytesAvailable() < sizeof(quint16))
                break;
            input >> nextBlockSize;
        }
        if(socket->bytesAvailable() < nextBlockSize)
            break;
        //message in in <input>, unpack it
        ChatMessageHeader *header = new ChatMessageHeader();
        header->unpack(input);
        ChatMessageType msgType = (ChatMessageType) header->messageType;
        delete header;
        switch (msgType)
        {
        case cmtChannelMessage:
            {
                ChannelMessage *msg = new ChannelMessage();
                msg->unpack(input);
                processMessage(msg);
                delete msg;
                break;
            }
        case cmtAuthorizationAnswer:
            {
                AuthorizationAnswer *msg = new AuthorizationAnswer();
                msg->unpack(input);
                processMessage(msg);
                delete msg;
                break;
            }
        default:
            {
                qDebug() << "Client received unknown-typed message";
                return;
            }
        }

    }
    nextBlockSize = 0;
}


void ChatClient::socketError(QAbstractSocket::SocketError error)
{
    QString strError =
            (error == QAbstractSocket::HostNotFoundError ?
                 "The host was not found." :
                 error == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     error == QAbstractSocket::ConnectionRefusedError ?
                         "The connect was refused." :
                         QString(tcpSocket->errorString()));
    emit errorOccured(strError);
    qDebug() << strError;
}

void ChatClient::sendChannelMessage(QString &rcvr, QString &body)
{
    ChannelMessage *msg = new ChannelMessage();
    msg->sender = username;
    msg->receiver = rcvr;
    msg->messageText = body;
    sendMessageToServer(msg);
    delete msg;
}

void ChatClient::sendMessageToServer(ChatMessageBody *msgBody)
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

    output << quint16(arrBlock.size() - sizeof(quint16));
    tcpSocket->write(arrBlock);
}

void ChatClient::processMessage(ChannelMessage *msg)
{
    qDebug() << "Processing channel message:" << msg->sender << msg->receiver << msg->messageText;
    QString message = QString("%1: %2")
                        .arg(msg->sender)
                        .arg(msg->messageText);
    emit messageToDisplay(message);
}

void ChatClient::processMessage(AuthorizationAnswer *msg)
{
    qDebug() << "Processing authorization answer:" << msg->authorizationResult;
    if (authorized)
    {
        qDebug() << "Allready authorizated, don't need to process that";
    }
    if (msg->authorizationResult)
    {
        authorized = true;
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

