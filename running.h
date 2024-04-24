#ifndef RUNNING_H
#define RUNNING_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QDebug>
#include <QtMath>

namespace Ui {
class running;
}

class running : public QWidget
{
    Q_OBJECT

public:
    explicit running(QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent *event);
    void drawOut(QPainter &p);
    void drawInt(QPainter &p);
    void drawPoint(QPainter &p);
    void drawText(QPainter &p);
    QString str1 = "--:--", str2 = "--:--";
    ~running();
public slots:
    void changeValue(qint64 value);

private:
    Ui::running *ui;
    void paintEvent(QPaintEvent *event);
    void getRectInfo();
    int minW;
    int BarWidth = 20;
    int value = 0;
};

#endif // RUNNING_H
