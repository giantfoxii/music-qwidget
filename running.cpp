#include "running.h"
#include "ui_running.h"

running::running(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::running)
{
    ui->setupUi(this);
    ui->horizontalSlider->hide();
}

void running::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    ui->horizontalSlider->move(20,height()-ui->horizontalSlider->height());
    getRectInfo();
    update();
}

void running::drawOut(QPainter &p)
{
    p.setPen(QPen(QColor(84, 204, 255),BarWidth));
    //p.drawEllipse(BarWidth,BarWidth,minW-2*BarWidth,minW-2*BarWidth);
    p.drawEllipse(BarWidth,BarWidth,minW-2*BarWidth,minW-2*BarWidth);
}

void running::drawInt(QPainter &p)
{
    QPen pen;
    pen.setWidth(BarWidth);
    QLinearGradient gradient(BarWidth,BarWidth,minW,minW);
    gradient.setColorAt(0,QColor(218,180,255));
    gradient.setColorAt(1,QColor(126,171,255));
    pen.setBrush(QBrush(gradient));
    pen.setCapStyle(Qt::RoundCap);                     // 形状：圆角
    pen.setStyle(Qt::SolidLine);
    pen.setJoinStyle(Qt::RoundJoin);
    p.setPen(pen);
    QRectF rect = QRectF(BarWidth,BarWidth,minW-2*BarWidth,minW-2*BarWidth);
    int startAngle = 92*16;                            // 起始角度位置，正上方偏左一点
    float spanAngle = value * 360 / 100 * 16 * -1;     // 负数为顺时针，正数为逆时针
    p.drawArc(rect,startAngle,spanAngle);
}

void running::drawPoint(QPainter &p)
{

}

void running::drawText(QPainter &p)
{
    QString str_value = str1 + "/" + str2;
    p.setPen(QPen(QColor(49, 177, 190)));
    QFont font("Microsoft YaHei UI",24);
    p.setFont(font);
    QFontMetrics fm(font);
    int w= fm.horizontalAdvance(str_value);
    p.drawText(minW/2-w/2,minW/2,str_value);
}

running::~running()
{
    delete ui;
}

void running::changeValue(qint64 value)
{
    this->value = value;
    update();
}

void running::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen;
    painter.setRenderHint(QPainter::Antialiasing);
    pen.setColor(QColor(128,222,233));
    pen.setWidth(2);

    painter.setPen(pen);

    drawOut(painter);
    drawInt(painter);
    drawText(painter);
}

void running::getRectInfo()
{
    minW = qMin(width(),ui->horizontalSlider->y()-5);
}
