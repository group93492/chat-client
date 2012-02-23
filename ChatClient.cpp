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
    msg->setUsername(this->username);
    msg->setPassword(this->password);
    sendMessageToServer(msg);
    delete msg;
}

void ChatClient::clientGotNewMessage()
{
    qDebug() << "Cleint received new message";
    QTcpSocket *socket = (QTcpSocket*)sender();
    QDataStream input(socket);
    input.setVersion(QDataStream::Qt_4_7);
    ChatMessageBody *newMessage;
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
        newMessage = ChatMessageSerializer::unpackMessage(input);
        // want to use something like this:
        // processMessage(pClientSocket, newMessage);
        // but this fails with typization error.
        // (in fact, i don't think it shoud fail, it looks correct to me)
        // i hope that dream will come true
        // but now could do only like this
        switch ((ChatMessageType) newMessage->getMessageType())
        {
        case cmtChannelMessage:
            {
                processMessage((ChannelMessage *) newMessage);
                break;
            }
        case cmtAuthorizationAnswer:
            {
                processMessage((AuthorizationAnswer *) newMessage);
                break;
            }
        default:
            {
                qDebug() << "Client received unknown-typed message";
                break;
            }
        }
        //looks fucking ugly, yeah?
        nextBlockSize = 0;
    }
    delete newMessage;
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
    msg->setSender(username);
    msg->setReceiver(rcvr);
    msg->setMessageText(body);
    sendMessageToServer(msg);
    delete msg;
}

void ChatClient::sendMessageToServer(ChatMessageBody *msgBody)
{
    QByteArray arrBlock;
    QDataStream output(&arrBlock, QIODevice::WriteOnly);
    output.setVersion(QDataStream::Qt_4_7);
    output << quint16(0);
    ChatMessageSerializer::packMessage(output, msgBody);
    output << quint16(arrBlock.size() - sizeof(quint16));
    tcpSocket->write(arrBlock);
}

void ChatClient::processMessage(ChannelMessage *msg)
{
    qDebug() << "Processing channel message:" << msg->getSender() << msg->getReceiver() << msg->getMessageText();
    QString message = "%1: %2";
    message.arg(msg->getSender()).arg(msg->getMessageText());
    emit messageToDisplay(message);
}

void ChatClient::processMessage(AuthorizationAnswer *msg)
{
    qDebug() << "Processing authorization answer:" << msg->getAuthorizationResult();
    if (authorized)
    {
        qDebug() << "Allready authorizated, don't need to process that";
    }
    if (msg->getAuthorizationResult())
    {
        authorized = true;
        //QString disp = "You passed authorization on server: " + tcpSocket->peerAddress().toString();
        //emit messageToDisplay(disp);
        emit clientAuthorized();
    }
    else
    {
        QString err = "Authorization problem: " + msg->getDenialReason();
        emit errorOccured(err);
    }
}

