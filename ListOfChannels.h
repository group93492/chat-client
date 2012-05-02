#ifndef LISTOFCHANNELS_H
#define LISTOFCHANNELS_H

#include <QDialog>
#include <QTableWidgetItem>
#include "ChatClient.h"

namespace Ui {
    class ListOfChannels;
}

class ListOfChannels : public QDialog
{
    Q_OBJECT

public:
    explicit ListOfChannels(QWidget *parent = 0);
    ~ListOfChannels();

private:
    Ui::ListOfChannels *ui;

protected:
    virtual void showEvent(QShowEvent *event);

signals:
    void requestJoinChannel(QString);

public slots:
    void setAllChannelsList(QMap<QString, QString> list);
    void getChannelJoinResult(QString channelName, bool result);

private slots:
    void on_joinButton_clicked();
};

#endif // LISTOFCHANNELS_H
