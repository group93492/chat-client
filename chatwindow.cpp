#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "ListOfChannels.h"
#include <QMessageBox>
#include <QDebug>

ChatWindow::ChatWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("icon.png"));
    m_client = new ChatClient(this);
    m_smiles = new smilesWidget(this);
    m_channelListDialog = new ListOfChannels(this);
    m_tabWidget = new ChatTabWidget(this, m_smiles->getSmiles());
    ui->chatLayout->addWidget(m_tabWidget);
    //connect part
    connect(m_client, SIGNAL(channelMsg(QString,QString,QString)), m_tabWidget, SLOT(appendMessage(QString,QString,QString)));
    connect(m_client, SIGNAL(channelSystemMsg(QString,QString)), m_tabWidget, SLOT(appendSystemMessage(QString,QString)));
    connect(m_client, SIGNAL(channelJoinResult(QString,bool)), m_channelListDialog, SLOT(getChannelJoinResult(QString,bool)));
    connect(m_client, SIGNAL(displayChannelList(QMap<QString,QString>, ChatClient::ChannelListType)), this, SLOT(processChannelList(QMap<QString,QString>, ChatClient::ChannelListType)));
    connect(m_client, SIGNAL(channelCreateResult(QString)), m_channelListDialog, SLOT(getChannelCreateResult(QString)));
    connect(m_client, SIGNAL(userList(QString,QStringList)), m_tabWidget, SLOT(setUserList(QString,QStringList)));
    connect(m_client, SIGNAL(channelThemeChange(QString,QString)), m_tabWidget, SLOT(changeTheme(QString,QString)));
    connect(m_tabWidget, SIGNAL(lastMessage(QString)), this, SLOT(setText(QString)));
    connect(m_tabWidget, SIGNAL(onNickClicked(QString)), this, SLOT(insertText(QString)));
    connect(m_tabWidget, SIGNAL(leaveChannel(QString)), m_client, SLOT(leaveChannel(QString)));
    connect(m_tabWidget, SIGNAL(themeChanged(QString,QString)), m_client, SLOT(changeChannelTheme(QString,QString)));
    connect(m_channelListDialog, SIGNAL(requestCreateChannel(QString,QString,QString)), m_client, SLOT(createChannelRequest(QString,QString,QString)));
    connect(m_channelListDialog, SIGNAL(requestJoinChannel(QString)), m_client, SLOT(joinChannelRequest(QString)));
    connect(this, SIGNAL(sendMessage(const QString&, const QString&)), m_client, SLOT(sendChannelMessage(const QString&, const QString&)));
    connect(this, SIGNAL(sendAllChannelsList(QMap<QString,QString>)), m_channelListDialog, SLOT(setAllChannelsList(QMap<QString,QString>)));
    connect(ui->smilesPushButton, SIGNAL(clicked()), m_smiles, SLOT(show()));
    connect(ui->allChannelsPushButton, SIGNAL(clicked()), m_channelListDialog, SLOT(show()));
    connect(ui->postButton, SIGNAL(clicked()), this, SLOT(postMessage()));
    connect(ui->messageEdit, SIGNAL(returnPressed()), this, SLOT(postMessage()));
    connect(m_smiles, SIGNAL(smileClicked(QString)), SLOT(insertText(QString)));


}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::closeEvent(QCloseEvent *event)
{
    m_client->shutdown();
    event->accept();
}

void ChatWindow::connectToServer(QString username, QString password, QTcpSocket *socket)
{
    setWindowTitle("Chat - " + username);
    m_tabWidget->setOwnerNick(username);
    m_client->setUserInfo(username, password);
    m_client->start(socket);
    this->show();
}

void ChatWindow::postMessage()
{
    QString body = ui->messageEdit->text();
    QString receiver = m_tabWidget->currentChannel();
    //if (receiver.isEmpty())
        //receiver = "main";
    emit sendMessage(receiver, body);
    ui->messageEdit->clear();
}

void ChatWindow::setText(QString text)
{
    ui->messageEdit->setText(text);
}

void ChatWindow::insertText(QString smileName)
{
    ui->messageEdit->insert(smileName);
}

void ChatWindow::processChannelList(QMap<QString, QString> list, ChatClient::ChannelListType type)
{
    if (type == ChatClient::listOfJoined)
        m_tabWidget->setChannelsList(list);
    else //type == ChatClient::listOfAll
        emit sendAllChannelsList(list);
}

void ChatWindow::getChannelJoinResult(QString channelName, bool result)
{
    emit replyJoinRequestResult(channelName, result);
    //update channel lists
    m_client->requestLists();
}

