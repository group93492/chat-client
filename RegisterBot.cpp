#include "RegisterBot.h"

RegisterBot::RegisterBot(QObject *parent) :
    QObject(parent)
{
}

RegisterBot::RegisterBot(QString &host, quint32 port, QString &username, QString &password):
    m_socket(NULL),
    m_nextBlockSize(0),
    m_host(host),
    m_port(port),
    m_username(username),
    m_password(password)
{
    m_socket = new QTcpSocket(this);
    connect(m_socket, SIGNAL(connected()), SLOT(botConnected()));
    connect(m_socket, SIGNAL(readyRead()), SLOT(botGotMessage()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(socketError(QAbstractSocket::SocketError)));
    m_socket->connectToHost(m_host, m_port);
}

RegisterBot::~RegisterBot()
{
    qDebug() << "deleting register bot";
    closeConnection();
}

void RegisterBot::closeConnection()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState)
        m_socket->disconnectFromHost();
}

void RegisterBot::botConnected()
{
    RegistrationRequest *msg = new RegistrationRequest();
    msg->username = m_username;
    msg->password = m_password;
    sendMessageToServer(msg);
    delete msg;
}

void RegisterBot::botGotMessage()
{
    QTcpSocket *socket = (QTcpSocket*)sender();
    if (socket == NULL)
        return;
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
        case cmtRegistrationAnswer:
            {
                RegistrationAnswer *msg = new RegistrationAnswer(input);
                processMessage(msg);
                delete msg;
                break;
            }
        default:
            {
                QString errorText = "Bot received unknown-typed message";
                emit errorOccured(errorText);
                break;
            }
        }
    }
    m_nextBlockSize = 0;
}

void RegisterBot::sendMessageToServer(ChatMessageBody *msgBody) const
{
    QByteArray arrBlock;
    QDataStream output(&arrBlock, QIODevice::WriteOnly);
    output.setVersion(QDataStream::Qt_4_7);
    output << quint16(0);
    ChatMessageHeader *header = new ChatMessageHeader(msgBody);
    header->pack(output);
    msgBody->pack(output);
    delete header;
    output << quint16(arrBlock.size() - sizeof(quint16));
    m_socket->write(arrBlock);
}

void RegisterBot::processMessage(RegistrationAnswer *msg)
{
    emit registrationCompleted(msg->registrationResult, msg->denialReason);
}

void RegisterBot::socketError(QAbstractSocket::SocketError error)
{
    QString strError =
            (error == QAbstractSocket::HostNotFoundError ?
                 "The host was not found." :
                 error == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     error == QAbstractSocket::ConnectionRefusedError ?
                         "The connect was refused." :
                         QString(m_socket->errorString()));
    emit errorOccured(strError);
}
