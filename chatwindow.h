#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QListView>
#include "smileswidgets.h"
#include "ChatClient.h"
#include "RegisterBot.h"
#include "ListOfChannels.h"


namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = 0);
    ~ChatWindow();
	
private:
    Ui::ChatWindow *ui;
    ChatClient *m_client;
    smilesWidget *m_smiles;
    QMap<QString, chatTextBrowser*> m_textBrowsersMap;
    ListOfChannels *m_channelsList;

signals:
    void sendMessage(const QString &, const QString &);

private slots:
    void clientError(const QString &);
    void displayMessage(const QString &msgText);
    void clientAuthorized();
    void postMessage();
    void addChannel(QString name);
    void removeChannel(int index);
    void addChannelMessage(QString channel, QString nick, QString message);
    void insertText(QString smileName);
    void lastMessageEdit(QString message);
public slots:
    void connectToServer(QString username, QString password, QTcpSocket *socket);
};

#endif // CHATWINDOW_H
