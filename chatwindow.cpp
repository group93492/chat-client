#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "ListOfChannels.h"

ChatWindow::ChatWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    ui->messageEdit->setEnabled(false);
    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(connectToServer()));
    connect(ui->postButton, SIGNAL(clicked()), this, SLOT(postMessage()));
    connect(ui->messageEdit, SIGNAL(returnPressed()), this, SLOT(postMessage()));
    m_client = new ChatClient(this);
    //connect part
    connect(m_client, SIGNAL(clientAuthorized()), this, SLOT(clientAuthorized()));
    connect(m_client, SIGNAL(errorOccured(const QString&)), this, SLOT(clientError(const QString&)));
    connect(m_client, SIGNAL(messageToDisplay(const QString&)), this, SLOT(displayMessage(const QString&)));
    connect(this, SIGNAL(sendMessage(const QString&, const QString&)), m_client, SLOT(sendChannelMessage(const QString&, const QString&)));
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::connectToServer()
{
    QString host = ui->hostEdit->text();
    quint16 port = ui->portEdit->text().toUInt();
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();
    m_client->setUserInfo(username, password);
    m_client->start(host, port);
}

void ChatWindow::clientError(const QString &errorText)
{
    QString errmsg = "Error: " + errorText;
    ui->ChatBrowser->append(errmsg);
}

void ChatWindow::displayMessage(const QString &msgText)
{
    ui->ChatBrowser->append(msgText);
}

void ChatWindow::clientAuthorized()
{
    ui->connectPropsGB->hide();
    ui->messageEdit->setEnabled(true);
    ui->ChatBrowser->append("Succesfully connected and authorized on server");
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
