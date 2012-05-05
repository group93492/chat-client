#include "ChannelThemeDialog.h"
#include "ui_ChannelThemeDialog.h"

ChannelThemeDialog::ChannelThemeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChannelThemeDialog)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("icon.png"));
}

ChannelThemeDialog::~ChannelThemeDialog()
{
    delete ui;
}

void ChannelThemeDialog::setTheme(QString theme)
{
    ui->lineEdit->setText(theme);
}

void ChannelThemeDialog::on_CancelButton_clicked()
{
    ui->lineEdit->clear();
    this->hide();
}

void ChannelThemeDialog::on_OKButton_clicked()
{
    QString theme = ui->lineEdit->text();
    emit onOkButtonClicked(theme);
    ui->lineEdit->clear();
    this->hide();
}
