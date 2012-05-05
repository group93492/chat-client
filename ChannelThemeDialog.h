#ifndef CHANNELTHEMEDIALOG_H
#define CHANNELTHEMEDIALOG_H

#include <QDialog>

namespace Ui {
class ChannelThemeDialog;
}

class ChannelThemeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ChannelThemeDialog(QWidget *parent = 0);
    ~ChannelThemeDialog();
    void setTheme(QString theme);
private:
    Ui::ChannelThemeDialog *ui;
signals:
    void onOkButtonClicked(QString theme);
private slots:
    void on_CancelButton_clicked();
    void on_OKButton_clicked();
};

#endif // CHANNELTHEMEDIALOG_H
