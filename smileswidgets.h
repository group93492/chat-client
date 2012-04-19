#ifndef SMILESWIDGETS_H
#define SMILESWIDGETS_H

#include <QApplication>
#include <QDialog>
#include <QGridLayout>
#include <QImage>
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <QTextBrowser>
#include <QDir>
#include <QTime>
#include <QTextBlock>
#include <math.h>

class chatTextBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    explicit chatTextBrowser(QWidget *parent = 0, QMap<QString, QString> *smilesMap = NULL);
    void setOwnerNick(QString nick);
private:
    QMap<QString, QString> *m_smilesMap;
    QString isSmile(quint16 index, QString msg);
    QString m_color;
    QString m_ownerColor;
    QString m_timePattern;
    QString m_ownerNick;
protected:
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseMoveEvent(QMouseEvent *ev);
public slots:
    void appendMessage(QString nick, QString msg);
    void appendMessage(QString msg);
    void setNickColor(QString color);
    void setTimePattern(QString pattern);
    void setOwnerColor(QString color);
signals:
    void onNickClicked(QString nick);
};

class smilesWidget : public QDialog
{
    Q_OBJECT
public:
    explicit smilesWidget(QWidget *parent = 0);
    ~smilesWidget();
    QMap<QString, QString>* getSmiles();
private:
    QMap<QString, QString> *m_smilesMap;
    QGridLayout *m_layout;
private slots:
    void smileName(QString name);
signals:
    void smileClicked(QString smileName);
};

class smileImage : public QWidget
{
    Q_OBJECT
public:
    smileImage(QString filename);
    void setSmileName(QString name);
private:
    QString m_name;
    QImage m_image;
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *);
signals:
    void smileImageClicked(QString name);
};

#endif // SMILESWIDGETS_H
