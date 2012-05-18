#include "AboutProgrammDialog.h"
#include "ui_AboutProgrammDialog.h"

AboutProgrammDialog::AboutProgrammDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutProgrammDialog)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("icon.png"));
}

AboutProgrammDialog::~AboutProgrammDialog()
{
    delete ui;
}

void AboutProgrammDialog::on_pushButton_clicked()
{
    this->hide();
}
