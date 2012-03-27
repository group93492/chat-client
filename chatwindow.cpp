#include "chatwindow.h"
#include "ui_chatwindow.h"

ChatWindow::ChatWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow),
    m_client(NULL),
    m_regBot(NULL)
{
    ui->setupUi(this);
    ui->messageEdit->setEnabled(false);
    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(connectToServer()));
    connect(ui->postButton, SIGNAL(clicked()), this, SLOT(postMessage()));
    connect(ui->messageEdit, SIGNAL(returnPressed()), this, SLOT(postMessage()));
    connect(ui->registerButton, SIGNAL(clicked()), this, SLOT(requestRegistration()));
    m_client = new ChatClient(this);
    //connect part
    connect(m_client, SIGNAL(clientAuthorized()), this, SLOT(clientAuthorized()));
    connect(m_client, SIGNAL(errorOccured(const QString&)), this, SLOT(clientError(const QString&)));
    connect(m_client, SIGNAL(messageToDisplay(const QString&)), this, SLOT(displayMessage(const QString&)));
    connect(m_client, SIGNAL(displayChannelList(QStringList&)), this, SLOT(channelListUpdate(QStringList&)));
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

void ChatWindow::channelListUpdate(QStringList &channels)
{
    QString text = (channels.isEmpty()) ? "You didnt join any channels" : "List of your channels: ";
    ui->channelsComboBox->clear();
    for (int i = 0; i < channels.count(); ++i)
    {
        ui->channelsComboBox->addItem(channels[i]);
        text += channels[i];
    }
    text += ".";
    displayMessage(text);
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
    QString receiver = ui->channelsComboBox->itemText(ui->channelsComboBox->currentIndex());
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

void ChatWindow::requestRegistration()
{
    if (m_regBot)
    {
        delete m_regBot;
        m_regBot = NULL;
    }
    QString host = ui->hostEdit->text();
    quint32 port = ui->portEdit->text().toUInt();
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();
    m_regBot = new RegisterBot(host, port, username, password);
    connect(m_regBot, SIGNAL(registrationCompleted(bool,QString&)), this, SLOT(registrationCompleted(bool,QString&)));
    connect(m_regBot, SIGNAL(errorOccured(QString&)), this, SLOT(registrationError(QString&)));
    ui->registerButton->setEnabled(false);
}

void ChatWindow::registrationCompleted(bool regResult, QString &denialReason)
{
    if (regResult)
    {
        ui->usernameEdit->clear();
        ui->passwordEdit->clear();
        QString msg = "Registration was successfully completed.";
        displayMessage(msg);
    }
    else
        displayMessage(denialReason);
    ui->registerButton->setEnabled(true);
    m_regBot->closeConnection();
}

void ChatWindow::registrationError(QString &errorMsg)
{
    ui->registerButton->setEnabled(true);
    QString msg = "Error during registration: " + errorMsg;
    displayMessage(msg);
    m_regBot->closeConnection();
}
