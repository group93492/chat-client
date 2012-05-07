#include "UserInfoDialog.h"
#include "ui_UserInfoDialog.h"

UserInfoDialog::UserInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserInfoDialog)
{
    ui->setupUi(this);
}

UserInfoDialog::~UserInfoDialog()
{
    delete ui;
}

void UserInfoDialog::on_pushButton_clicked()
{
    this->hide();
}
