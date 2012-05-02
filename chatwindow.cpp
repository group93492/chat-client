#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "ListOfChannels.h"
#include <QMessageBox>

ChatWindow::ChatWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    connect(ui->postButton, SIGNAL(clicked()), this, SLOT(postMessage()));
    connect(ui->messageEdit, SIGNAL(returnPressed()), this, SLOT(postMessage()));
    m_client = new ChatClient(this);
    m_smiles = new smilesWidget(this);
    m_channelListDialog = new ListOfChannels(this);
    //connect part
    connect(ui->smilesPushButton, SIGNAL(clicked()), m_smiles, SLOT(show()));
    connect(m_client, SIGNAL(clientAuthorized()), this, SLOT(clientAuthorized()));
    connect(m_client, SIGNAL(errorOccured(const QString&)), this, SLOT(clientError(const QString&)));
    connect(m_client, SIGNAL(channelMsg(QString,QString,QString)), this, SLOT(addChannelMessage(QString,QString,QString)));
    connect(m_client, SIGNAL(userList(QString,QStringList)), this, SLOT(setChannelUsers(QString,QStringList)));
    connect(m_client, SIGNAL(channelSystemMsg(QString,QString)), this, SLOT(addChannelSystemMessage(QString,QString)));
    connect(this, SIGNAL(sendMessage(const QString&, const QString&)), m_client, SLOT(sendChannelMessage(const QString&, const QString&)));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(removeChannelTab(int)));
    connect(m_smiles, SIGNAL(smileClicked(QString)), SLOT(insertText(QString)));
    connect(ui->allChannelsPushButton, SIGNAL(clicked()), m_channelListDialog, SLOT(show()));
    connect(m_channelListDialog, SIGNAL(requestJoinChannel(QString)), m_client, SLOT(joinChannelRequest(QString)));
    connect(m_client, SIGNAL(channelJoinResult(QString,bool)), m_channelListDialog, SLOT(getChannelJoinResult(QString,bool)));
    connect(m_client,
            SIGNAL(displayChannelList(QMap<QString,QString>, ChatClient::ChannelListType)),
            this,
            SLOT(processChannelList(QMap<QString,QString>, ChatClient::ChannelListType)));
    connect(this, SIGNAL(sendAllChannelsList(QMap<QString,QString>)), m_channelListDialog, SLOT(setAllChannelsList(QMap<QString,QString>)));
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
    m_client->setUserInfo(username, password);
    m_client->start(socket);
    //

    chatTextBrowser *textBrowser = new chatTextBrowser(ui->tabWidget->currentWidget(), m_smiles->getSmiles());
    textBrowser->setOwnerNick(username);
    QHBoxLayout *layout = new QHBoxLayout();
    QListWidget *listwidget = new QListWidget();
    layout->addWidget(textBrowser, 3);
    layout->addWidget(listwidget,1);
    m_textBrowsersMap.insert("main", textBrowser);
    m_listWidgetsMap.insert("main", listwidget);
    ui->tabWidget->currentWidget()->setLayout(layout);
    connect(textBrowser, SIGNAL(lastMessage(QString)), this, SLOT(lastMessageEdit(QString)));
    connect(textBrowser, SIGNAL(onNickClicked(QString)), SLOT(insertText(QString)));

    //
    this->show();
}

void ChatWindow::clientError(const QString &errorText)
{
    QString errmsg = "Error: " + errorText;
    addChannelSystemMessage("main", errmsg);
}

void ChatWindow::displayMessage(const QString &msgText)
{
}

void ChatWindow::clientAuthorized()
{
    ui->messageEdit->setEnabled(true);
}

void ChatWindow::postMessage()
{
    QString body = ui->messageEdit->text();
    QString receiver = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if (receiver.isEmpty())
        receiver = "main";
    emit sendMessage(receiver, body);
    ui->messageEdit->clear();
}

void ChatWindow::addChannelTab(QString name)
{
    QWidget *widget = new QWidget(ui->tabWidget);
    chatTextBrowser *textBrowser = new chatTextBrowser(ui->tabWidget->currentWidget(), m_smiles->getSmiles());
    textBrowser->setOwnerNick(m_client->username());
    QHBoxLayout *layout = new QHBoxLayout();
    QListWidget *listWidget = new QListWidget();
    layout->addWidget(textBrowser, 3);
    layout->addWidget(listWidget,1);
    widget->setLayout(layout);
    ui->tabWidget->addTab(widget, name);
    m_textBrowsersMap.insert(name, textBrowser);
    m_listWidgetsMap.insert(name, listWidget);
    connect(textBrowser, SIGNAL(lastMessage(QString)), this, SLOT(lastMessageEdit(QString)));
    connect(textBrowser, SIGNAL(onNickClicked(QString)), SLOT(insertText(QString)));
}

void ChatWindow::removeChannelTab(int index)
{
    if(index != 0)
    {
        if (QMessageBox::question(this,
                                  "Exit confirmation.",
                                  "Are you sure you want to leave this channel?",
                                  QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
            return;
        m_client->leaveChannel(ui->tabWidget->tabText(index));
        m_textBrowsersMap.remove(ui->tabWidget->tabText(index));
        ui->tabWidget->removeTab(index);
    }
}

void ChatWindow::addChannelMessage(QString channel, QString nick, QString message)
{
    if(m_textBrowsersMap.contains(channel))
        m_textBrowsersMap.value(channel)->appendMessage(nick, message);
}

void ChatWindow::addChannelSystemMessage(QString channel, QString message)
{
    if(m_textBrowsersMap.contains(channel))
        m_textBrowsersMap.value(channel)->appendSystemMessage(message);
}

void ChatWindow::insertText(QString smileName)
{
    ui->messageEdit->insert(smileName);
}

void ChatWindow::lastMessageEdit(QString message)
{
    ui->messageEdit->setText(message);
}

void ChatWindow::setChannelUsers(QString channelname, QStringList list)
{
    if(m_listWidgetsMap.contains(channelname))
    {
        m_listWidgetsMap.value(channelname)->clear();
        m_listWidgetsMap.value(channelname)->addItems(list);
    }
}

void ChatWindow::processChannelList(QMap<QString, QString> list, ChatClient::ChannelListType type)
{
    if (type == ChatClient::listOfJoined)
        setMyChannelList(list);
    else //type == ChatClient::listOfAll
        emit sendAllChannelsList(list);
}

void ChatWindow::getChannelJoinResult(QString channelName, bool result)
{
    emit replyJoinRequestResult(channelName, result);
    //update channel lists
    m_client->requestLists();
}

void ChatWindow::setMyChannelList(QMap<QString, QString> list)
{
    QMap<QString, QString>::iterator channel = list.begin();
    for(;channel != list.end(); ++channel)
        if (!m_textBrowsersMap.contains(channel.key()))
            addChannelTab(channel.key());
}
