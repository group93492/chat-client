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
