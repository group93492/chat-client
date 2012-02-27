#include "chatwindow.h"
#include "ui_chatwindow.h"

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
    connect(m_client, SIGNAL(errorOccured(QString&)), this, SLOT(clientError(QString&)));
    connect(m_client, SIGNAL(messageToDisplay(QString&)), this, SLOT(displayMessage(QString&)));
    connect(this, SIGNAL(sendMessage(QString&,QString&)), m_client, SLOT(sendChannelMessage(QString&,QString&)));
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

void ChatWindow::clientError(QString &errorText)
{
    QString errmsg = "Error: " + errorText;
    ui->ChatBrowser->append(errmsg);
}

void ChatWindow::displayMessage(QString &msgText)
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
    QString receiver = "*";
    QString body = ui->messageEdit->text();
    emit sendMessage(receiver, body);
    ui->messageEdit->clear();
}
