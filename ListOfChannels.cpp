#include "ListOfChannels.h"
#include "ui_ListOfChannels.h"
#include <QDebug>

ListOfChannels::ListOfChannels(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListOfChannels)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(2);
}

ListOfChannels::~ListOfChannels()
{
    delete ui;
}

void ListOfChannels::setChannelsList(QMap<QString, QString> list)
{
    QTableWidgetItem *item;
    ui->tableWidget->setRowCount(list.size());
    QMap<QString, QString>::iterator i = list.begin();
    quint8 row = 0;
    for(;i != list.end(); ++i)
    {
        item = new QTableWidgetItem(i.key());
        ui->tableWidget->setItem(row, 0, item);
        item = new QTableWidgetItem(i.value());
        ui->tableWidget->setItem(row, 1, item);
        ++row;
    }
}
