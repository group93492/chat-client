#include "StatusDialog.h"
#include "ui_StatusDialog.h"

StatusDialog::StatusDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatusDialog)
{
    ui->setupUi(this);
    setWindowTitle("New status");
    setWindowIcon(QIcon("icon.png"));
}

StatusDialog::~StatusDialog()
{
    delete ui;
}

void StatusDialog::on_OKpushButton_clicked()
{
    emit statusChanged(ui->statusEdit->text());
    this->hide();
    ui->statusEdit->clear();
}

void StatusDialog::on_CancelPushButton_clicked()
{
    ui->statusEdit->clear();
    this->hide();
}
