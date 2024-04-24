#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QDir>
#include <QRandomGenerator>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


    //界面美化
    QIcon qicon(":/icon/sun.png");
    setWindowIcon(qicon);
    setWindowTitle("FoxMusic");


    //创建播放器
    audioOutput = new QAudioOutput(this);
    mediaPlayer = new QMediaPlayer(this);
    mediaPlayer->setAudioOutput(audioOutput);
    //connect(ui->play,&QPushButton::clicked,this,&Widget::on_play_clicked);


    //音量按钮初始化
    ui->widget_3->hide();
    ui->verticalSlider->setValue(100);
    connect(ui->verticalSlider,&QSlider::sliderMoved,audioOutput,[=](int val)
    {
        float real = (float)val/100;
        audioOutput->setVolume(real);
    });

    //connect(ui->add,&QPushButton::clicked,this,&Widget::on_add_clicked);

    //拖动滑块改变进度
    connect(ui->horizontalSlider,&QSlider::sliderMoved,mediaPlayer,&QMediaPlayer::setPosition);

    //获取当前媒体总时长
    connect(mediaPlayer,&QMediaPlayer::durationChanged,[=](qint64 dur){
        QString temp = QString("%1 : %2").arg(dur/1000/60,2,10,QChar('0')).arg(dur/1000%60,2,10,QChar('0'));
        ui->totallabel->setText(temp);
        ui->widget_5->str2 = temp;
        ui->horizontalSlider->setRange(0,dur);
    });
    //获取当前时长
    connect(mediaPlayer,&QMediaPlayer::positionChanged,[=](qint64 pos){
        QString temp = QString("%1 : %2").arg(pos/1000/60,2,10,QChar('0')).arg(pos/1000%60,2,10,QChar('0'));
        ui->nowtime->setText(temp);
        ui->widget_5->str1 = temp;
        ui->horizontalSlider->setValue(pos);
    });

    //初始化进度窗口
    connect(ui->horizontalSlider,&QSlider::valueChanged,ui->widget_5,[=](qint64 val){
        if(mediaPlayer->duration())
        {
            val = val*100 / mediaPlayer->duration();
            ui->widget_5->changeValue(val);
        }
    });
    //按照模式进行下一首播放
    connect(mediaPlayer,&QMediaPlayer::playbackStateChanged,[=](QMediaPlayer::PlaybackState newState){
        if(newState == QMediaPlayer::StoppedState && mediaPlayer->position() == mediaPlayer->duration()){
            selectPlayModel();
        }
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_voice_clicked()
{
    if(ui->widget_3->isVisible())
        ui->widget_3->hide();
    else
        ui->widget_3->show();
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(this->rect(),QPixmap(":/icon/huitailang.jpg"),QRect());
}
void Widget::mousePressEvent(QMouseEvent *event)
{

    if(event->button() == Qt::LeftButton)
    {
        isLeftButton = true;
        Pos = event->pos();
    }

}
void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if(isLeftButton)
    {
        isLeftButton = false;
    }
    event->ignore();
}
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint now;
    if(isLeftButton)
    {
        now = this->pos() + event->pos() - Pos;
        this->move(now);
    }
}



void Widget::on_play_clicked()
{
    if(playList.empty())
        return;
    switch(mediaPlayer->playbackState())
    {
    case QMediaPlayer::StoppedState:
    {
        CurPlayIndex = ui->listWidget->currentRow();
        mediaPlayer->setSource(playList[CurPlayIndex]);
        changePlayIcon();
        mediaPlayer->play();
        break;
    }
    case QMediaPlayer::PlayingState:
        changePlayIcon();
        mediaPlayer->pause();

        break;
    case QMediaPlayer::PausedState:
        changePlayIcon();
        mediaPlayer->play();
        break;
    }
}


void Widget::on_add_clicked()
{
    auto Path = QFileDialog::getExistingDirectory(this,"请选择音乐文件","F:/");
    QDir dir(Path);
    auto musicList = dir.entryList(QStringList()<<"*.mp3"<<"*.wav");
    playList.clear();
    while(ui->listWidget->takeItem(0) != nullptr);
    ui->listWidget->addItems(musicList);
    ui->listWidget->setCurrentRow(0);
    for(auto &file:musicList)
        playList.append(QUrl::fromLocalFile(Path +"/"+file));
}

void Widget::changePlayIcon()
{
    int playState = mediaPlayer->playbackState();
    if(playState == QMediaPlayer::PlayingState)
    {
        ui->play->setStyleSheet("border-image: url(:/icon/icon_play.png)");
    }
    else
    {
        ui->play->setStyleSheet("border-image: url(:/icon/play.png)");
    }
}
void Widget::changeMusic(int cur)
{
    int playState = mediaPlayer->playbackState();//playState 0是停止 1是播放 2是暂停
    if(cur < 0)
    {
        CurPlayIndex = 0;
        mediaPlayer->stop();
        ui->play->setStyleSheet("border-image: url(:/icon/play.png)");
        return;
    }
    else if(cur >= playList.size())
    {
        CurPlayIndex = playList.size() - 1;
        mediaPlayer->stop();
        ui->play->setStyleSheet("border-image: url(:/icon/icon_play.png)");
        return;
    }
    ui->listWidget->setCurrentRow(cur);
    mediaPlayer->setSource(playList[cur]);
    if(playState == QMediaPlayer::PlayingState)
        mediaPlayer->play();
    else if(playState == QMediaPlayer::PausedState)
        mediaPlayer->pause();
    else
    {
        changePlayIcon();
        mediaPlayer->play();
    }
}
void Widget::on_last_clicked()
{
    --CurPlayIndex;
    changeMusic(CurPlayIndex);
}


void Widget::on_next_clicked()
{
    ++CurPlayIndex;
    changeMusic(CurPlayIndex);
}


void Widget::on_playmodel_clicked()
{
    ++playModel;
    playModel %= 4;
    switch(playModel)
    {
    case 0:
        ui->playmodel->setStyleSheet("border-image:url(:/icon/order.png)");
        break;
    case 1:
        ui->playmodel->setStyleSheet("border-image:url(:/icon/loop.png)");
        break;
    case 2:
        ui->playmodel->setStyleSheet("border-image:url(:/icon/single_loop.png)");
        break;
    case 3:
        ui->playmodel->setStyleSheet("border-image:url(:/icon/random.png)");
    }
}

void Widget::selectPlayModel()
{
    switch(playModel)
    {
    case 0:
        ++CurPlayIndex;
        changeMusic(CurPlayIndex);
        break;
    case 1:
        ++CurPlayIndex;
        CurPlayIndex %= playList.size();
        changeMusic(CurPlayIndex);
        break;
    case 2:
        changeMusic(CurPlayIndex);
        break;
    case 3:
    {
        QRandomGenerator rand;
        rand.seed(QTime(0,0,0).secsTo(QTime::currentTime()));
        CurPlayIndex = rand.bounded(playList.size());
        changeMusic(CurPlayIndex);
    }
    break;
    }


}
