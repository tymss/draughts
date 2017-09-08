#include "dlg.h"

dlg::dlg(QWidget *parent):
    QDialog(parent)
{
    this->setFixedSize(400,300);
    this->move((QApplication::desktop()->width()-width())/2,(QApplication::desktop()->height()-height())/2);
    setWindowFlags(windowFlags()&~Qt::WindowCloseButtonHint&~Qt::WindowContextHelpButtonHint);
    ip=new QLineEdit(this);
    ip->setFixedSize(200,40);
    ipL=new QLabel("IP地址:",this);
    ipL->setFixedSize(80,40);
    ipL->move(20,60);
    ip->move(100,60);
    port=new QLineEdit(this);
    portL=new QLabel("端口号:",this);
    port->setFixedSize(80,40);
    portL->setFixedSize(80,40);
    portL->move(20,120);
    port->move(100,120);
    connecting=new QPushButton("连接",this);
    connecting->setFixedSize(200,50);
    closing=new QPushButton("退出",this);
    closing->setFixedSize(200,50);
    connecting->move(100,170);
    closing->move(100,230);
    connect(closing,SIGNAL(clicked(bool)),this,SIGNAL(to_close()));
    connect(connecting,SIGNAL(clicked(bool)),this,SLOT(connection()));
    QPalette pal;
    pal.setBrush(QPalette::Background,QBrush(QPixmap(":/image/image1.jpg")));
    setPalette(pal);
    QFont font;
    font.setFamily("华文琥珀");
    font.setPointSize(10);
    this->setFont(font);
    font.setFamily("幼圆");
    font.setBold(true);
    ip->setFont(font);
    port->setFont(font);
    ip->setStyleSheet("border:3px solid #000000;border-radius:10px;");
    port->setStyleSheet("border:3px solid #000000;border-radius:10px;");
    connecting->setStyleSheet("QPushButton{background-color:black;\
                              color: white;   border-radius: 10px;  border: 0px;}"
                              "QPushButton:hover{background-color:rgb(207,207,207); color: black;}"
                              "QPushButton:pressed{background-color:gray;color:white}");
    closing->setStyleSheet("QPushButton{background-color:black;\
                           color: white;   border-radius: 10px;  border: 0px;}"
                           "QPushButton:hover{background-color:rgb(207,207,207); color: black;}"
                           "QPushButton:pressed{background-color:gray;color:white}");
}
void dlg::connection()
{
    QString ipstr,portstr;
    ipstr=ip->text();
    portstr=port->text();
    emit to_connect(ipstr,portstr);
}
