#include "ListOfChannels.h"
#include "ui_ListOfChannels.h"
#include <QDebug>

ListOfChannels::ListOfChannels(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListOfChannels)
{
    ui->setupUi(this);
    connect(&m_dialog, SIGNAL(createChannel(QString,QString,QString)), this, SLOT(replyCreateSignal(QString,QString,QString)));
    connect(ui->createButton, SIGNAL(clicked()), &m_dialog, SLOT(show()));
}

ListOfChannels::~ListOfChannels()
{
    delete ui;
}

void ListOfChannels::showEvent(QShowEvent *event)
{
    ui->joinStatusLabel->clear();
}

void ListOfChannels::setAllChannelsList(QMap<QString, QString> list)
{
    QTableWidgetItem *item;
    ui->channelTable->clear();
    ui->channelTable->setRowCount(list.size());
    QMap<QString, QString>::iterator i = list.begin();
    quint8 row = 0;
    for(;i != list.end(); ++i)
    {
        item = new QTableWidgetItem(i.key());
        ui->channelTable->setItem(row, 0, item);
        item = new QTableWidgetItem(i.value());
        ui->channelTable->setItem(row, 1, item);
        ++row;
    }
}

void ListOfChannels::getChannelJoinResult(QString channelName, bool result)
{
    //notify user that his join was succesfull. or not
    QString newStatus;
    if (result)
    {
        newStatus = "You've sucessfully joined channel " + channelName + ".";
    }
    else
    {
        newStatus = "Your attempt to join channel " + channelName + " wasn't successfull.";
    }
    ui->joinStatusLabel->setText(newStatus);
}

void ListOfChannels::getChannelCreateResult(QString str)
{
    ui->joinStatusLabel->setText(str);
}

void ListOfChannels::replyCreateSignal(QString name, QString topic, QString description)
{
    emit requestCreateChannel(name, topic, description);
}

void ListOfChannels::on_joinButton_clicked()
{
    //send signal requestJoinChannel if row is checked
    QModelIndexList indexes = ui->channelTable->selectionModel()->selectedRows();
    if (indexes.isEmpty())
    {
        ui->joinStatusLabel->setText("You must select any channel to join it.");
        return;
    }
    QModelIndex index = indexes[0];
    QString channelName = ui->channelTable->item(index.row(), index.column())->text();
    emit requestJoinChannel(channelName);
    ui->joinStatusLabel->setText("Join request has been sent.");
}
