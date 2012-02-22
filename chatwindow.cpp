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
    client = new ChatClient(this);
    //connect part
    connect(client, SIGNAL(clientAuthorized()), this, SLOT(clientAuthorized()));
    connect(client, SIGNAL(errorOccured(QString&)), this, SLOT(clientError(QString&)));
    connect(client, SIGNAL(messageToDisplay(QString&)), this, SLOT(displayMessage(QString&)));
    connect(this, SIGNAL(sendMessage(QString&,QString&)), client, SLOT(sendInformationalMessage(QString&,QString&)));
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
    client->setUserInfo(username, password);
    client->start(host, port);
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
}

void ChatWindow::postMessage()
{
    QString receiver = "*";
    QString body = ui->messageEdit->text();
    emit sendMessage(receiver, body);
    ui->messageEdit->clear();
}
