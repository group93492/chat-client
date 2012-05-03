#include "createchanneldialog.h"
#include "ui_createchanneldialog.h"

CreateChannelDialog::CreateChannelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateChannelDialog)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("icon.png"));
}

CreateChannelDialog::~CreateChannelDialog()
{
    delete ui;
}

void CreateChannelDialog::on_cancelButton_clicked()
{
    ui->nameEdit->clear();
    ui->topicEdit->clear();
    ui->descriptionEdit->clear();
    this->hide();
}

void CreateChannelDialog::on_createButton_clicked()
{
    if(ui->nameEdit->text() == "" || ui->topicEdit->text() == "" || ui->descriptionEdit->text() == "")
    {
        QMessageBox msg(QMessageBox::Information, "Notify", "Please, fill all fields.", QMessageBox::Ok);
        msg.exec();
    }
    else
    {
        emit createChannel(ui->nameEdit->text(), ui->topicEdit->text(), ui->descriptionEdit->text());
        this->hide();
        ui->nameEdit->clear();
        ui->topicEdit->clear();
        ui->descriptionEdit->clear();
    }
}
