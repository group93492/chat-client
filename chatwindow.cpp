#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "ListOfChannels.h"

ChatWindow::ChatWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    connect(ui->postButton, SIGNAL(clicked()), this, SLOT(postMessage()));
    connect(ui->messageEdit, SIGNAL(returnPressed()), this, SLOT(postMessage()));
    m_client = new ChatClient(this);
    m_smiles = new smilesWidget(this);
    //connect part
    connect(ui->smilesPushButton, SIGNAL(clicked()), m_smiles, SLOT(show()));
    connect(m_client, SIGNAL(clientAuthorized()), this, SLOT(clientAuthorized()));
    connect(m_client, SIGNAL(errorOccured(const QString&)), this, SLOT(clientError(const QString&)));
    connect(m_client, SIGNAL(channelMsg(QString,QString,QString)), this, SLOT(addChannelMessage(QString,QString,QString)));
    connect(this, SIGNAL(sendMessage(const QString&, const QString&)), m_client, SLOT(sendChannelMessage(const QString&, const QString&)));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(removeChannel(int)));
    connect(m_smiles, SIGNAL(smileClicked(QString)), SLOT(insertText(QString)));
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::connectToServer(QString username, QString password, QTcpSocket *socket)
{
    m_client->setUserInfo(username, password);
    m_client->start(socket);
    //
    chatTextBrowser *textBrowser = new chatTextBrowser(ui->tabWidget->currentWidget(), m_smiles->getSmiles());
    textBrowser->setOwnerNick(username);
    QHBoxLayout *layout = new QHBoxLayout();
    QListView *listView = new QListView();
    layout->addWidget(textBrowser, 3);
    layout->addWidget(listView,1);
    m_textBrowsersMap.insert("main", textBrowser);
    ui->tabWidget->currentWidget()->setLayout(layout);
    connect(textBrowser, SIGNAL(lastMessage(QString)), this, SLOT(lastMessageEdit(QString)));
    connect(textBrowser, SIGNAL(onNickClicked(QString)), SLOT(insertText(QString)));
    //
    this->show();
}

void ChatWindow::clientError(const QString &errorText)
{
    QString errmsg = "Error: " + errorText;
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

void ChatWindow::on_pushButton_2_clicked()
{
    ListOfChannels listofchannels;
    listofchannels.exec();
}

void ChatWindow::addChannel(QString name)
{
    QWidget *widget = new QWidget(ui->tabWidget);
    chatTextBrowser *textBrowser = new chatTextBrowser(ui->tabWidget->currentWidget(), m_smiles->getSmiles());
    textBrowser->setOwnerNick(m_client->username());
    QHBoxLayout *layout = new QHBoxLayout();
    QListView *listView = new QListView();
    layout->addWidget(textBrowser, 3);
    layout->addWidget(listView,1);
    widget->setLayout(layout);
    ui->tabWidget->addTab(widget, name);
    m_textBrowsersMap.insert(name, textBrowser);
    connect(textBrowser, SIGNAL(lastMessage(QString)), this, SLOT(lastMessageEdit(QString)));
    connect(textBrowser, SIGNAL(onNickClicked(QString)), SLOT(insertText(QString)));
}

void ChatWindow::removeChannel(int index)
{
    if(index != 0)
    {
        m_textBrowsersMap.remove(ui->tabWidget->tabText(index));
        ui->tabWidget->removeTab(index);
    }
}

void ChatWindow::addChannelMessage(QString channel, QString nick, QString message)
{
    m_textBrowsersMap.value(channel)->appendMessage(nick, message);
}

void ChatWindow::insertText(QString smileName)
{
    ui->messageEdit->insert(smileName);
}

void ChatWindow::lastMessageEdit(QString message)
{
    ui->messageEdit->setText(message);
}
