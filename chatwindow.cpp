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
    connect(m_client, SIGNAL(errorOccured(const QString&)), this, SLOT(clientError(const QString&)));
    connect(m_client, SIGNAL(messageToDisplay(const QString&)), this, SLOT(displayMessage(const QString&)));
    connect(this, SIGNAL(sendMessage(const QString&, const QString&)), m_client, SLOT(sendChannelMessage(const QString&, const QString&)));
    connect(ui->disconnectButton, SIGNAL(clicked()), m_client, SLOT(sendDisconnectMessage()));
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
    ui->disconnectButton->setEnabled(true);
}

void ChatWindow::postMessage()
{
    QString receiver = "*";
    QString body = ui->messageEdit->text();
    emit sendMessage(receiver, body);
    ui->messageEdit->clear();
}

void ChatWindow::on_disconnectButton_clicked()
{
    ui->connectPropsGB->show();
    ui->messageEdit->setEnabled(false);
    ui->disconnectButton->setEnabled(false);
    ui->ChatBrowser->append("Disconnect from server");
}
