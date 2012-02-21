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
        ChatMessageBody *newMessage = ChatMessageSerializer::unpackMessage(input);
        // want to use something like this:
        // processMessage(pClientSocket, newMessage);
        // but this fails with typization error.
        // (in fact, i don't think it shoud fail, it looks correct to me)
        // i hope that dream will come true
        // but now could do only like this
        switch ((ChatMessageType) newMessage->messageType)
        {
        case cmtInformationalMessage:
            {
                processMessage((InformationalMessage *) newMessage);
                break;
            }
        case cmtAuthorizationRequest:
            {
                processMessage((AuthorizationAnswer *) newMessage);
                break;
            }
        }
        //looks fucking ugly, yeah?
        delete newMessage;
        nextBlockSize = 0;
    }
}

void ChatClient::socketError(QAbstractSocket::SocketError error)
{
    QString strError = "Error: " +
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

void ChatClient::sendInformationalMessage(QString &body, QString &rcvr)
{
    InformationalMessage *msg = new InformationalMessage();
    msg->sender = username;
    msg->receiver = rcvr;
    msg->messageBody = body;
    sendMessageToServer(msg);
    delete msg;
}

void ChatClient::sendMessageToServer(ChatMessageBody *msgBody)
{
    QByteArray arrBlock;
    QDataStream output(&arrBlock, QIODevice::WriteOnly);
    output.setVersion(QDataStream::Qt_4_7);
    ChatMessageSerializer::packMessage(output, msgBody);
    tcpSocket->write(arrBlock);
}

void ChatClient::processMessage(InformationalMessage *msg)
{
    qDebug() << "Processing informational message:" << msg->sender << msg->receiver << msg->messageBody;
    QString message = "%1: %2";
    message.arg(msg->sender).arg(msg->messageBody);
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
        QString err = "Authorization error: " + msg->authorizationReason;
        emit errorOccured(err);
    }
}
