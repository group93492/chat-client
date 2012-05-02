#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QListWidget>
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
    QMap<QString, QListWidget*> m_listWidgetsMap;
    ListOfChannels *m_channelsList;
    void setMyChannelList(QMap<QString,QString> list);

protected:
    virtual void closeEvent(QCloseEvent *event);

signals:
    void sendMessage(const QString &, const QString &);
    void sendAllChannelsList(QMap<QString,QString>);
    void requestUserList(QString);


private slots:
    void clientError(const QString &);
    void displayMessage(const QString &msgText);
    void clientAuthorized();
    void postMessage();
    void addChannelTab(QString name);
    void removeChannelTab(int index);
    void addChannelMessage(QString channel, QString nick, QString message);
    void addChannelSystemMessage(QString channel, QString message);
    void insertText(QString smileName);
    void lastMessageEdit(QString message);
    void setChannelUsers(QString channelname, QStringList list);
    void processChannelList(QMap<QString,QString> list, ChatClient::ChannelListType type);

public slots:
    void connectToServer(QString username, QString password, QTcpSocket *socket);
};

#endif // CHATWINDOW_H
