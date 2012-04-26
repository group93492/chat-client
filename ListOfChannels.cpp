#include "ListOfChannels.h"
#include "ui_ListOfChannels.h"

ListOfChannels::ListOfChannels(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListOfChannels)
{
    ui->setupUi(this);
}

ListOfChannels::~ListOfChannels()
{
    delete ui;
}
