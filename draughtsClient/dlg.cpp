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
    ipL->move(20,40);
    ip->move(100,40);
    port=new QLineEdit(this);
    portL=new QLabel("端口号:",this);
    port->setFixedSize(80,40);
    portL->setFixedSize(80,40);
    portL->move(20,100);
    port->move(100,100);
    connecting=new QPushButton("连接",this);
    connecting->setFixedSize(200,50);
    closing=new QPushButton("退出",this);
    closing->setFixedSize(200,50);
    connecting->move(100,170);
    closing->move(100,230);
    connect(closing,SIGNAL(clicked(bool)),this,SIGNAL(to_close()));
    connect(connecting,SIGNAL(clicked(bool)),this,SLOT(connection()));
}
void dlg::connection()
{
    QString ipstr,portstr;
    ipstr=ip->text();
    portstr=port->text();
    emit to_connect(ipstr,portstr);
}
