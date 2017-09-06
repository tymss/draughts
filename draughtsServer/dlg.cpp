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
    closing=new QPushButton("退出",this);
    closing->setFixedSize(200,60);
    closing->move(100,190);
    ip->setEnabled(false);
    port->setEnabled(false);
    port->setText("8999");
    QString strIp;
    QString localHostName=QHostInfo::localHostName();
    QList<QHostAddress> list=QHostInfo::fromName(localHostName).addresses();
    int nListSize=list.size();
    for (int i = 0; i < nListSize; i++)
    {
        if (list.at(i) != QHostAddress::LocalHost &&
            list.at(i).toIPv4Address())
        {
            strIp = list.at(i).toString();
            break;
        }
    }
    ip->setText(strIp);
    connect(closing,SIGNAL(clicked(bool)),this,SIGNAL(to_close()));

}

