#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    chosen.setX(-1);
    chosen.setY(-1);
    gm=new game;
    setFixedSize(1000,700);
    giveup=new QPushButton("认输",this);
    draw=new QPushButton("求和",this);
    giveup->setFixedSize(200,80);
    draw->setFixedSize(200,80);
    giveup->move(750,50);
    draw->move(750,200);
    info=new QTextEdit(this);
    info->setFixedSize(200,250);
    info->move(750,400);
    infol=new QLabel("游戏记录:",this);
    infol->setFixedSize(200,40);
    infol->move(750,360);
    dialog=new dlg(this);
    QPalette pal;
    pal.setBrush(QPalette::Background,QBrush(QPixmap(":/image/image2.jpg")));
    setPalette(pal);
    draw->setStyleSheet("QPushButton{background-color:black;\
                        color: white;   border-radius: 10px;  border: 0px;}"
                        "QPushButton:hover{background-color:rgb(207,207,207); color: black;}"
                        "QPushButton:pressed{background-color:gray;color:white}");
    giveup->setStyleSheet("QPushButton{background-color:black;\
                          color: white;   border-radius: 10px;  border: 0px;}"
                          "QPushButton:hover{background-color:rgb(207,207,207); color: black;}"
                          "QPushButton:pressed{background-color:gray;color:white}");
    QFont font;
    font.setFamily("华文琥珀");
    font.setPointSize(10);
    giveup->setFont(font);
    draw->setFont(font);
    infol->setFont(font);
    font.setFamily("幼圆");
    font.setBold(true);
    info->setFont(font);
    info->setStyleSheet("border:3px solid #000000;border-radius:10px;");
    connect(this,SIGNAL(trans(int,int,int,int)),gm,SLOT(deal(int,int,int,int)));
    connect(gm,SIGNAL(display()),this,SLOT(repaint()));
    connect(gm,SIGNAL(showMes(QString)),this,SLOT(showMes(QString)));
    connect(gm,SIGNAL(yourturn()),this,SLOT(myturn()));
    connect(gm,SIGNAL(finish()),this,SLOT(finish()));
    connect(gm,SIGNAL(defeat(int)),this,SLOT(fail(int)));
    connect(gm,SIGNAL(success(int)),this,SLOT(win(int)));
    connect(giveup,SIGNAL(clicked(bool)),gm,SLOT(giveup()));
    connect(giveup,SIGNAL(clicked(bool)),this,SLOT(givingup()));
    connect(gm,SIGNAL(accept()),this,SLOT(nowin()));
    connect(draw,SIGNAL(clicked(bool)),gm,SLOT(beg()));
    connect(dialog,SIGNAL(to_close()),this,SLOT(deleteLater()));
    connect(dialog,SIGNAL(to_connect(QString,QString)),gm,SLOT(connecting(QString,QString)));
    connect(gm,SIGNAL(con_success()),dialog,SLOT(close()));
    dialog->exec();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter *paint=new QPainter(this);
    paint->setPen(QPen(Qt::black,3));
    paint->drawRect(50,50,600,600);
    for(int j=0;j<10;j++)
    for(int i=0;i<10;i++)
    {
        if(j%2==0)
        {
            if(i%2==0)
                paint->setBrush(QBrush(Qt::white));
            else
                paint->setBrush(QBrush(Qt::gray));
        }
        else
        {
            if(i%2!=0)
                paint->setBrush(QBrush(Qt::white));
            else
                paint->setBrush(QBrush(Qt::gray));
        }
        paint->drawRect(50+i*60,50+j*60,60,60);
    }
    paint->setPen(QPen(Qt::white,1));
    for(int i=0;i<10;i++)
    for(int j=0;j<10;j++)
    {
        if(gm->chessman(i,j))
        {
            if(gm->chessman(i,j)==1)
                paint->setBrush(QBrush(Qt::darkRed));
            else
                paint->setBrush(QBrush(Qt::black));
            paint->drawEllipse(QPoint(80+i*60,80+j*60),27,27);
            if(gm->isking(i,j))
            {
                QPixmap pm(":/image/icon.png");
                paint->drawPixmap(50+60*i,40+60*j,60,85,pm);
            }
            if(yourturn&&!gm->available(i,j).isEmpty())
            {
                paint->setPen(QPen(Qt::white,4));
                paint->drawLine(80+i*60,70+j*60,80+i*60,90+j*60);
                paint->drawLine(70+i*60,80+j*60,90+i*60,80+j*60);
                paint->setPen(QPen(Qt::white,1));
            }
        }
    }
    if(chosen!=QPoint(-1,-1)&&!gm->available(chosen.x(),chosen.y()).isEmpty())
    {
        paint->setBrush(Qt::NoBrush);
        paint->setPen(QPen(Qt::green,3));
        paint->drawEllipse(QPoint(80+chosen.x()*60,80+chosen.y()*60),27,27);
        for(int i=0;i<gm->available(chosen.x(),chosen.y()).size();i++)
        {
            paint->drawEllipse(QPoint(80+gm->available(chosen.x(),chosen.y()).at(i).x()*60,
                                      80+gm->available(chosen.x(),chosen.y()).at(i).y()*60),
                               27,27);
        }
    }

}
void Widget::mousePressEvent(QMouseEvent *event)
{
    if(yourturn&&event->button()==Qt::LeftButton&&event->pos().x()>=50&&
            event->pos().x()<650&&event->pos().y()>=50&&event->pos().y()<650)
    {
        int cx=(event->pos().x()-50)/60;
        int cy=(event->pos().y()-50)/60;
        if(chosen!=QPoint(-1,-1))
        {
            for(int i=0;i<gm->available(chosen.x(),chosen.y()).size();i++)
            {
                if(gm->available(chosen.x(),chosen.y()).at(i)==QPoint(cx,cy))
                {
                    emit trans(chosen.x(),chosen.y(),cx,cy);
                }
            }
        }
        chosen.setX(cx);
        chosen.setY(cy);
        repaint();
    }
    else
    {
        return QWidget::mousePressEvent(event);
    }

}

void Widget::showMes(QString mes)
{
    info->append(mes);
}

void Widget::myturn()
{
    yourturn=1;
    chosen.setX(-1);
    chosen.setY(-1);
    repaint();
}

void Widget::finish()
{
    yourturn=0;
    chosen.setX(0);
    chosen.setY(0);
    repaint();
}

void Widget::givingup()
{
    fail(1);
}

void Widget::fail(int w)
{
    finish();
    sound=new QSound(":/sound/fail.wav",this);
    sound->play();
    QString sinfo;
    info->append("你输了");
    if(w==0)
        sinfo="你失去了所有棋子!";
    else if(w==2)
        sinfo="你没有可以移动的棋子!";
    else
        sinfo="你选择了认输!";
    QMessageBox mesbox(QMessageBox::NoIcon,QString("游戏失败"),sinfo);
    mesbox.exec();
}

void Widget::win(int w)
{
    finish();
    sound=new QSound(":/sound/win.wav",this);
    sound->play();
    QString sinfo;
    info->append("你赢了");
    if(w==0)
        sinfo="对方失去了所有棋子!";
    else if(w==2)
        sinfo="对方没有可以移动的棋子!";
    else
        sinfo="对方选择了认输!";
    QMessageBox mesbox(QMessageBox::NoIcon,QString("游戏胜利"),sinfo);
    mesbox.exec();
}
void Widget::nowin()
{
    finish();
    sound=new QSound(":/sound/draw.wav",this);
    sound->play();
    QMessageBox mesbox(QMessageBox::NoIcon,QString("游戏结束"),QString("本局为和局。"));
    mesbox.exec();
}
