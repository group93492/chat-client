#include "ChatWidgets.h"

GeneralChatWidget::GeneralChatWidget(QWidget *parent) :
    QWidget(parent)
{
    m_theme = new QLabel();
    m_textBrowser = new chatTextBrowser();
    m_userList = new QListWidget();
    m_label = new QLabel("Users in channel: " + QString::number(m_userList->count()));
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
    m_users = list;
    m_users.sort();
    m_userList->addItems(m_users);
    m_label->setText("Users in channel: " + QString::number(m_users.count()));
}

void GeneralChatWidget::setUserStatus(QString nick, QString status)
{
    if(m_users.contains(nick))
    {
        if(status != "")
            m_userList->item(m_users.indexOf(nick))->setText(nick + "[" + status + "]");
        else
            m_userList->item(m_users.indexOf(nick))->setText(nick);
    }
}

void GeneralChatWidget::appendMessage(QString nick, QString msg)
{
    m_textBrowser->appendMessage(nick, msg);
}
