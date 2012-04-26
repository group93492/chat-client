#ifndef LISTOFCHANNELS_H
#define LISTOFCHANNELS_H

#include <QDialog>

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
};

#endif // LISTOFCHANNELS_H
