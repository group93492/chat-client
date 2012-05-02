#ifndef LISTOFCHANNELS_H
#define LISTOFCHANNELS_H

#include <QDialog>
#include <QTableWidgetItem>

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

public slots:
    void setAllChannelsList(QMap<QString, QString> list);
    void getchannelJoinResult(QString channelName, bool result);
};

#endif // LISTOFCHANNELS_H
