#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QMediaPlayer>
#include <QMouseEvent>
#include <QPoint>
#include <QFrame>
#include <QAudioOutput>
#include <QFileDialog>
#include <QDir>
#include <QUrl>
#include "QDebug"
#include <QIcon>
#include <QPainter>
#include <QString>
#include <QHBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void changePlayIcon();
    void changeMusic(int cur);
    void selectPlayModel();
    QList<QUrl> playList;
    ~Widget();

private slots:
    void on_voice_clicked();

    void on_play_clicked();

    void on_add_clicked();

    void on_last_clicked();

    void on_next_clicked();

    void on_playmodel_clicked();

private:
    Ui::Widget *ui;
    //移动窗口
    QPoint Pos;
    bool isLeftButton = false;

    QAudioOutput *audioOutput;
    QMediaPlayer *mediaPlayer;
    int CurPlayIndex;
    int playModel;

};
#endif // WIDGET_H
