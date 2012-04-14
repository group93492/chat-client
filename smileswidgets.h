#ifndef SMILESWIDGETS_H
#define SMILESWIDGETS_H

#include <QDialog>
#include <QGridLayout>
#include <QImage>
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <math.h>
#include <QTextBrowser>
#include <QDir>

class chatTextBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    explicit chatTextBrowser(QWidget *parent = 0, QMap<QString, QString> *smilesMap = NULL);
    void appendMessage(QString msg);
private:
    QMap<QString, QString> *m_smilesMap;
    QString isSmile(quint16 index, QString msg);
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
