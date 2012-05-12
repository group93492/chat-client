#include "OwnerProfile.h"
#include "ui_OwnerProfile.h"

OwnerProfile::OwnerProfile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OwnerProfile)
{
    ui->setupUi(this);
    setWindowTitle("Your profile");
    setWindowIcon(QIcon("icon.png"));
}

OwnerProfile::~OwnerProfile()
{
    delete ui;
}

void OwnerProfile::on_closeButton_clicked()
{
    ui->infLabel->clear();
    this->hide();
}

void OwnerProfile::on_saveButton_clicked()
{
    emit changeInfo(ui->nicknameEdit->text(), ui->infoEdit->toPlainText());
    this->hide();
}

void OwnerProfile::on_changeButton_clicked()
{
    if(ui->curPasswordEdit->text() != "" && ui->newPasswordEdit->text() != "")
    {
        emit changePassword(ui->curPasswordEdit->text(), ui->newPasswordEdit->text());
        ui->curPasswordEdit->clear();
        ui->newPasswordEdit->clear();
    }
    else
        ui->infLabel->setText("Please, fill all records");
}

void OwnerProfile::setResult(QString text)
{
    ui->infLabel->setText(text);
}

void OwnerProfile::setInfo(QString username, QString info)
{
    ui->nicknameEdit->setText(username);
    ui->infoEdit->setText(info);
}
