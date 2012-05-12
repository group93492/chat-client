#include "UserInfoDialog.h"
#include "ui_UserInfoDialog.h"

UserInfoDialog::UserInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserInfoDialog)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("icon.png"));
}

UserInfoDialog::~UserInfoDialog()
{
    delete ui;
}

void UserInfoDialog::showInfo(QString username, QString info)
{
    setWindowTitle(username + " - info");
    ui->usernameEdit->setText(username);
    ui->infoEdit->setText(info);
    this->show();
}

void UserInfoDialog::on_okButton_clicked()
{
    this->hide();
}
