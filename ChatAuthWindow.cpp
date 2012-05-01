#include "ChatAuthWindow.h"
#include "ui_ChatAuthWindow.h"
#include "ConnectionProperties.h"

ChatAuthWindow::ChatAuthWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatAuthWindow)
{
    ui->setupUi(this);
    m_host = "localhost";
    m_port = 33033;
}

ChatAuthWindow::~ChatAuthWindow()
{
    delete ui;
}

void ChatAuthWindow::setConnectionProperty(QString host, quint32 port)
{
    m_host = host;
    m_port = port;
}

void ChatAuthWindow::on_registerButton_clicked()
{
    if(ui->passwordEdit->text() == "" || ui->usernameEdit->text() == "")
    {
        ui->infLabel->setText("Username and password must be non-empty.");
        return;
    }
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();
    m_bot = new RegisterBot(m_host, m_port, username, password, true);
    connect(m_bot, SIGNAL(errorOccured(QString)), ui->infLabel, SLOT(setText(QString)));
    connect(m_bot, SIGNAL(registrationCompleted(bool,QString&)), this, SLOT(registrationHandler(bool,QString&)));
}

void ChatAuthWindow::registrationHandler(bool registrationResult, QString &denialReason)
{
    if(!registrationResult)
        ui->infLabel->setText(denialReason);
    else
    {
        ui->infLabel->setText("Registration was successful");
        m_bot->closeConnection();
        delete m_bot;
    }
}

void ChatAuthWindow::on_propertiesButton_clicked()
{
    ConnectionProperties connection_window;
    connect(&connection_window, SIGNAL(properties(QString,quint32)), this, SLOT(setConnectionProperty(QString,quint32)));
    connection_window.exec();
}

void ChatAuthWindow::replyClientAuthorized(QTcpSocket *socket)
{
    emit authorized(ui->usernameEdit->text(), ui->passwordEdit->text(), socket);
    this->hide();
}

void ChatAuthWindow::on_connectButton_clicked()
{
    if(ui->passwordEdit->text() == "" || ui->usernameEdit->text() == "")
    {
        ui->infLabel->setText("Username and password must be non-empty.");
        return;
    }
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();
    m_bot = new RegisterBot(m_host, m_port, username, password, false);
    connect(m_bot, SIGNAL(errorOccured(QString)), ui->infLabel, SLOT(setText(QString)));
    connect(m_bot, SIGNAL(authorized(QTcpSocket*)), this, SLOT(replyClientAuthorized(QTcpSocket*)));
}
