#include "ChatAuthWindow.h"
#include "ui_ChatAuthWindow.h"
#include "ConnectionProperties.h"

ChatAuthWindow::ChatAuthWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatAuthWindow)
{
    ui->setupUi(this);
}

ChatAuthWindow::~ChatAuthWindow()
{
    delete ui;
}

void ChatAuthWindow::on_pushButton_clicked()
{
    ConnectionProperties connection_window;
    connection_window.exec();
}
