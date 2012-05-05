#include "ChatWidgets.h"
#include <QDebug>

GeneralChatWidget::GeneralChatWidget(QWidget *parent) :
    QWidget(parent)
{
    m_users = new QStringList;
    m_theme = new ThemeLabel(this);
    m_textBrowser = new chatTextBrowser(this);
    m_userList = new UserListWidget(this, m_users);
    m_label = new QLabel("Users in channel: " + QString::number(m_userList->count()), this);
    m_theme->setFrameShape(QFrame::StyledPanel);
    m_theme->setFrameShadow(QFrame::Raised);
    m_theme->setAlignment(Qt::AlignHCenter);
    QVBoxLayout *generalLayout = new QVBoxLayout();
    QHBoxLayout *layoutOne = new QHBoxLayout();
    QHBoxLayout *layoutTwo = new QHBoxLayout();
    layoutOne->addWidget(m_theme, 3);
    layoutOne->addWidget(m_label, 1);
    layoutTwo->addWidget(m_textBrowser, 3);
    layoutTwo->addWidget(m_userList, 1);
    generalLayout->addLayout(layoutOne);
    generalLayout->addLayout(layoutTwo, 10);
    this->setLayout(generalLayout);
}

void GeneralChatWidget::setUserList(QStringList list)
{
    list.sort();
    m_users->clear();
    m_users->append(list);
    m_userList->addItems(list);
    m_label->setText("Users in channel: " + QString::number(m_users->count()));
}

void GeneralChatWidget::setUserStatus(QString nick, QString status)
{
    if(m_users->contains(nick))
    {
        if(status != "")
            m_userList->item(m_users->indexOf(nick))->setText(nick + "[" + status + "]");
        else
            m_userList->item(m_users->indexOf(nick))->setText(nick);
    }
}

void GeneralChatWidget::appendMessage(QString nick, QString msg)
{
    m_textBrowser->appendMessage(nick, msg);
}

UserListWidget::UserListWidget(QWidget *parent, QStringList *list) :
    QListWidget(parent)
{
    m_list = list;
}

void UserListWidget::contextMenuEvent(QContextMenuEvent *e)
{
    if(this->itemAt(e->pos()) != NULL)
    {
        QMenu *menu = new QMenu();
        menu->addAction("Send private message", this, SLOT(sendPMSignal()));
        menu->addAction("Profile", this, SLOT(sendInfoSignal()));
        menu->exec(e->globalPos());
        delete menu;
    }
}

void UserListWidget::sendInfoSignal()
{
    QString nick = m_list->value(this->row(this->selectedItems().first()));
    emit onUserInformationClicked(nick);
}

void UserListWidget::sendPMSignal()
{
    QString nick = m_list->value(this->row(this->selectedItems().first()));
    emit onPrivateMessageClicked(nick);
}

ThemeLabel::ThemeLabel(QWidget *parent) :
    QLabel(parent)
{
}

void ThemeLabel::changeTheme(QString theme)
{
    setText(theme);
    m_dialog.hide();
}

void ThemeLabel::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        m_dialog.setTheme(this->text());
        m_dialog.show();
        connect(&m_dialog, SIGNAL(onOkButtonClicked(QString)), this, SLOT(changeTheme(QString)));
    }
}
