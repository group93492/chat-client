#include "ConnectionProperties.h"
#include "ui_ConnectionProperties.h"

ConnectionProperties::ConnectionProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionProperties)
{
    ui->setupUi(this);
}

ConnectionProperties::~ConnectionProperties()
{
    emit properties(ui->hostEdit->text(), ui->portEdit->text().toUInt());
    delete ui;
}
