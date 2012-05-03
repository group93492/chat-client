#ifndef LISTOFCHANNELS_H
#define LISTOFCHANNELS_H

#include <QDialog>
#include <QTableWidgetItem>
#include "ChatClient.h"
#include "createchanneldialog.h"

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
    CreateChannelDialog m_dialog;
protected:
    virtual void showEvent(QShowEvent *event);

signals:
    void requestJoinChannel(QString);
    void requestCreateChannel(QString, QString, QString);

public slots:
    void setAllChannelsList(QMap<QString, QString> list);
    void getChannelJoinResult(QString channelName, bool result);
    void getChannelCreateResult(QString str);
    void replyCreateSignal(QString name, QString topic, QString description);

private slots:
    void on_joinButton_clicked();
};

#endif // LISTOFCHANNELS_H
