#include "ChatAuthWindow.h"
#include "ui_ChatAuthWindow.h"

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
