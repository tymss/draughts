#include "game.h"
#include <QDebug>
#include <QCoreApplication>
int game::chessman(int x, int y)
{
    return myState[x][y].team;
}

QList<QPoint> game::available(int x, int y)
{
    QList<QPoint> pl;
    for(int i=0;i<mt[x][y]->child->size();i++)
        pl.append(mt[x][y]->child->at(i)->p);
    return pl;
}

void game::deal(int x1,int y1,int x2,int y2)
{
    int first=y1*5+x1/2+1,next=y2*5+x2/2+1;
    emit showMes(QString("%1-%2").arg(first).arg(next));
    if(x1-x2==1||x1-x2==-1)
    {
        myState[x2][y2]=myState[x1][y1];
        myState[x1][y1].team=0;
    }
    else
    {
        int xx=x1,yy=y1;
        while(xx!=x2)
        {
            xx+=(x2-x1)/qAbs(x2-x1);
            yy+=(y2-y1)/qAbs(y2-y1);
            if(myState[xx][yy].team)
            {
                myState[xx][yy].team=0;
                myState[xx][yy].isking=0;
            }
        }
        myState[x2][y2]=myState[x1][y1];
        myState[x1][y1].team=0;
    }
    bool de=0;
    for(int i=0;i<10;i++)
    for(int j=0;j<10;j++)
    {
        if(myState[i][j].team==2)
            de=1;
    }
    if(!de)
    {
        emit sendMes("defeat");
        emit success(0);
        return;
    }
    if(myState[x2][y2].team==1)
    {
        sendMes(QString("%1,%2,%3,%4").arg(x1).arg(y1).arg(x2).arg(y2));
        for(int i=0;i<mt[x1][y1]->child->size();i++)
        {
            if(mt[x1][y1]->child->at(i)->p==QPoint(x2,y2))
            {
                mt[x2][y2]=mt[x1][y1]->child->at(i);
                mt[x1][y1]->child->clear();
                break;
            }
        }
        for(int i=0;i<10;i++)
        for(int j=0;j<10;j++)
        {
            if(i!=x2||j!=y2)
            {
                for(int k=0;k<mt[i][j]->child->size();k++)
                    delete mt[i][j]->child->at(k);
                mt[i][j]->child->clear();
                mt[i][j]->killing=0;
            }
        }
        if(mt[x2][y2]->child->isEmpty())
        {
            mt[x2][y2]->killing=0;
            if(y2==0)
                myState[x2][y2].isking=1;
            sendMes("yourturn");
            emit showMes("对方的回合:");
            emit finish();
        }
    }
    emit display();
}
game::game()
{
    for(int j=0;j<10;j++)
    for(int i=0;i<10;i++)
    {
        myState[i][j].team=0;
        if(j==0||j==2)
            if(i%2!=0)
                myState[i][j].team=2;
        if(j==1||j==3)
            if(i%2==0)
                myState[i][j].team=2;
        if(j==6||j==8)
            if(i%2!=0)
                myState[i][j].team=1;
        if(j==7||j==9)
            if(i%2==0)
                myState[i][j].team=1;
        lastState[i][j]=myState[i][j];
        mt[i][j]=new methodTree(i,j);
    }
    for(int i=1;i<10;i+=2)
    {
        if(i!=9)
        {
            methodTree *m1,*m2;
            m1=new methodTree(i-1,5);
            mt[i][6]->child->append(m1);
            m2=new methodTree(i+1,5);
            mt[i][6]->child->append(m2);
        }
        else
        {
            methodTree *m1;
            m1=new methodTree(i-1,5);
            mt[i][6]->child->append(m1);
        }
    }
    socket=new QTcpSocket(this);
    connect(socket,SIGNAL(readyRead()),this,SLOT(readMes()));
}


void game::readMes()
{
    QString mes;
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_4);
    if(blocksize==0)
    {
        if(socket->bytesAvailable()<(int)sizeof(quint16)) return;
        in>>blocksize;
    }
    if(socket->bytesAvailable()<blocksize) return;
    blocksize=0;
    in>>mes;
    if(mes=="myturn")
    {
        emit showMes("对方的回合:");
    }
    else if(mes=="yourturn")
    {
        for(int i=0;i<10;i++)
        {
            if(myState[i][9].team==2)
                myState[i][9].isking=1;
        }
        getmethod();
        emit showMes("你的回合:");
        emit yourturn();
    }
    else if(mes=="defeat")
    {
        emit defeat(0);
    }
    else if(mes=="giveup")
    {
        emit success(1);
    }
    else if(mes=="nomethod")
    {
        emit success(2);
    }
    else if(mes=="beg")
    {
        QMessageBox mesbox(QMessageBox::NoIcon, "求和申请", "对方申请求和", QMessageBox::Yes | QMessageBox::No,NULL);
        if(mesbox.exec()==QMessageBox::Yes)
        {
            emit accept();
            sendMes("accept");
        }
        else
            sendMes("refuse");
    }
    else if(mes=="refuse")
    {
        emit showMes("对方拒绝了求和申请");
    }
    else if(mes=="accept")
    {
        emit accept();
    }
    else
    {
        int x1=mes.mid(0,1).toInt();
        int y1=mes.mid(2,1).toInt();
        int x2=mes.mid(4,1).toInt();
        int y2=mes.mid(6,1).toInt();
        x1=9-x1; y1=9-y1; x2=9-x2; y2=9-y2;
        deal(x1,y1,x2,y2);
    }
}

void game::beg()
{
    sendMes("beg");
    emit showMes("你发出了求和申请。");
}

void game::giveup()
{
    sendMes("giveup");
}

void game::connecting(QString ip, QString port)
{
    socket->connectToHost(QHostAddress(ip),port.toInt());
    if(socket->waitForConnected())
        emit con_success();
}

void game::sendMes(QString mes)
{
    QByteArray order;
    QDataStream out(&order,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_4);
    out<<(quint16)0;
    out<<mes;
    out.device()->seek(0);
    out<<(quint16)(order.size()-sizeof(quint16));
    socket->write(order);
    socket->waitForBytesWritten();
    QTime dieTime = QTime::currentTime().addMSecs(100);
    while( QTime::currentTime()<dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

bool game::isking(int x, int y)
{
    return myState[x][y].isking;
}

game::~game()
{
    for(int i=0;i<10;i++)
    for(int j=0;j<10;j++)
    {
        delete mt[i][j];
    }
}

void game::getmethod()
{
    int Max=1;
    QList<QPoint> pl;
    for(int i=0;i<10;i++)
    for(int j=0;j<10;j++)
    {
        if(myState[i][j].team==1)
        {
            if(myState[i][j].isking)
                trymethod(i,j,mt[i][j],0,1);
            else
                trymethod(i,j,mt[i][j],0,0);
            mt[i][j]->getl();
            if(Max<=mt[i][j]->lenth)
            {
                Max=mt[i][j]->lenth;
                if(pl.isEmpty()||(mt[pl.last().x()][pl.last().y()]->lenth==Max&&
                        mt[pl.last().x()][pl.last().y()]->killing==mt[i][j]->killing))
                {
                    pl<<QPoint(i,j);
                }
                else if(mt[pl.last().x()][pl.last().y()]->lenth<Max||
                        (mt[pl.last().x()][pl.last().y()]->killing<mt[i][j]->killing))
                {
                    while(!pl.isEmpty())
                        pl.removeLast();
                    pl<<QPoint(i,j);
                }
            }
        }
    }
    if(pl.isEmpty())
    {
        sendMes("nomethod");
        emit defeat(2);
    }
    else
    {
        for(int i=0;i<10;i++)
        for(int j=0;j<10;j++)
        {
            if(myState[i][j].team==1)
            {
                if(pl.indexOf(QPoint(i,j))==-1)
                {
                    while(!mt[i][j]->child->isEmpty())
                    {
                        delete mt[i][j]->child->last();
                        mt[i][j]->child->removeLast();
                    }
                }
            }
        }
    }
}


void game::trymethod(int x, int y, methodTree *tree,bool ischild,bool isking)
{
    if(!isking)
    {
        if(x-2>=0&&y-2>=0&&myState[x-1][y-1].team==2&&myState[x-2][y-2].team==0)
        {
            mt[x][y]->killing=1;
            methodTree *m=new methodTree(x-2,y-2,1);
            tree->child->append(m);
            myState[x-2][y-2]=myState[x][y];
            myState[x][y].team=0;
            myState[x-1][y-1].team=3;
            trymethod(x-2,y-2,m,1,0);
            myState[x-1][y-1].team=2;
            myState[x][y]=myState[x-2][y-2];
            myState[x-2][y-2].team=0;
        }
        if(x-2>=0&&y+2<=9&&myState[x-1][y+1].team==2&&myState[x-2][y+2].team==0)
        {
            mt[x][y]->killing=1;
            methodTree *m=new methodTree(x-2,y+2,1);
            tree->child->append(m);
            myState[x-2][y+2]=myState[x][y];
            myState[x][y].team=0;
            myState[x-1][y+1].team=3;
            trymethod(x-2,y+2,m,1,0);
            myState[x-1][y+1].team=2;
            myState[x][y]=myState[x-2][y+2];
            myState[x-2][y+2].team=0;
        }
        if(x+2<=9&&y-2>=0&&myState[x+1][y-1].team==2&&myState[x+2][y-2].team==0)
        {
            mt[x][y]->killing=1;
            methodTree *m=new methodTree(x+2,y-2,1);
            tree->child->append(m);
            myState[x+2][y-2]=myState[x][y];
            myState[x][y].team=0;
            myState[x+1][y-1].team=3;
            trymethod(x+2,y-2,m,1,0);
            myState[x+1][y-1].team=2;
            myState[x][y]=myState[x+2][y-2];
            myState[x+2][y-2].team=0;
        }
        if(x+2<=9&&y+2<=9&&myState[x+1][y+1].team==2&&myState[x+2][y+2].team==0)
        {
            mt[x][y]->killing=1;
            methodTree *m=new methodTree(x+2,y+2,1);
            tree->child->append(m);
            myState[x+2][y+2]=myState[x][y];
            myState[x][y].team=0;
            myState[x+1][y+1].team=3;
            trymethod(x+2,y+2,m,1,0);
            myState[x+1][y+1].team=2;
            myState[x][y]=myState[x+2][y+2];
            myState[x+2][y+2].team=0;
        }
        if(tree->child->isEmpty())
        {
            if(ischild)
                return;
            else
            {
                if(y>0&&x-1>=0&&myState[x-1][y-1].team==0)
                {
                    methodTree *m=new methodTree(x-1,y-1);
                    tree->child->append(m);
                }
                if(y>0&&x+1<=9&&myState[x+1][y-1].team==0)
                {
                    methodTree *m=new methodTree(x+1,y-1);
                    tree->child->append(m);
                }
            }
        }
    }
    else
    {
        int xx=x,yy=y;
        int cx=-1,cy=-1;
        while(xx>1&&yy>1)
        {
            if(myState[xx-1][yy-1].team==1||(myState[xx-1][yy-1].team&&myState[xx-2][yy-2].team))
                break;
            if(myState[xx-1][yy-1].team==2&&myState[xx-2][yy-2].team==0)
            {
                mt[x][y]->killing=1;
                myState[xx-1][yy-1].team=3;
                cx=xx-1;
                cy=yy-1;
                xx=xx-2;
                yy=yy-2;
                break;
            }
            xx--;
            yy--;
        }
        while(xx>=0&&yy>=0&&cx!=-1&&myState[xx][yy].team==0)
        {
            myState[xx][yy]=myState[x][y];
            myState[x][y].team=0;
            myState[x][y].isking=0;
            methodTree *m=new methodTree(xx,yy,1);
            tree->child->append(m);
            trymethod(xx,yy,m,1,1);
            myState[x][y]=myState[xx][yy];
            myState[xx][yy].team=0;
            myState[xx][yy].isking=0;
            xx--;
            yy--;
        }
        if(cx!=-1)
            myState[cx][cy].team=2;
        xx=x,yy=y,cx=-1,cy=-1;
        while(xx>1&&yy<8)
        {
            if(myState[xx-1][yy+1].team==1||(myState[xx-1][yy+1].team&&myState[xx-2][yy+2].team))
                break;
            if(myState[xx-1][yy+1].team==2&&myState[xx-2][yy+2].team==0)
            {
                mt[x][y]->killing=1;
                myState[xx-1][yy+1].team=3;
                cx=xx-1;
                cy=yy+1;
                xx=xx-2;
                yy=yy+2;
                break;
            }
            xx--;
            yy++;
        }
        while(xx>=0&&yy<=9&&cx!=-1&&myState[xx][yy].team==0)
        {
            myState[xx][yy]=myState[x][y];
            myState[x][y].team=0;
            myState[x][y].isking=0;
            methodTree *m=new methodTree(xx,yy,1);
            tree->child->append(m);
            trymethod(xx,yy,m,1,1);
            myState[x][y]=myState[xx][yy];
            myState[xx][yy].team=0;
            myState[xx][yy].isking=0;
            xx--;
            yy++;
        }
        if(cx!=-1)
            myState[cx][cy].team=2;
        xx=x,yy=y,cx=-1,cy=-1;
        while(xx<8&&yy<8)
        {
            if(myState[xx+1][yy+1].team==1||(myState[xx+1][yy+1].team&&myState[xx+2][yy+2].team))
                break;
            if(myState[xx+1][yy+1].team==2&&myState[xx+2][yy+2].team==0)
            {
                mt[x][y]->killing=1;
                myState[xx+1][yy+1].team=3;
                cx=xx+1;
                cy=yy+1;
                xx=xx+2;
                yy=yy+2;
                break;
            }
            xx++;
            yy++;
        }
        while(xx<=9&&yy<=9&&cx!=-1&&myState[xx][yy].team==0)
        {
            myState[xx][yy]=myState[x][y];
            myState[x][y].team=0;
            myState[x][y].isking=0;
            methodTree *m=new methodTree(xx,yy,1);
            tree->child->append(m);
            trymethod(xx,yy,m,1,1);
            myState[x][y]=myState[xx][yy];
            myState[xx][yy].team=0;
            myState[xx][yy].isking=0;
            xx++;
            yy++;
        }
        if(cx!=-1)
            myState[cx][cy].team=2;
        xx=x,yy=y,cx=-1,cy=-1;
        while(xx<8&&yy>1)
        {
            if(myState[xx+1][yy-1].team==1||(myState[xx+1][yy-1].team&&myState[xx+2][yy-2].team))
                break;
            if(myState[xx+1][yy-1].team==2&&myState[xx+2][yy-2].team==0)
            {
                mt[x][y]->killing=1;
                myState[xx+1][yy-1].team=3;
                cx=xx+1;
                cy=yy-1;
                xx=xx+2;
                yy=yy-2;
                break;
            }
            xx++;
            yy--;
        }
        while(xx<=9&&yy>=0&&cx!=-1&&myState[xx][yy].team==0)
        {
            myState[xx][yy]=myState[x][y];
            myState[x][y].team=0;
            myState[x][y].isking=0;
            methodTree *m=new methodTree(xx,yy,1);
            tree->child->append(m);
            trymethod(xx,yy,m,1,1);
            myState[x][y]=myState[xx][yy];
            myState[xx][yy].team=0;
            myState[xx][yy].isking=0;
            xx++;
            yy--;
        }
        if(cx!=-1)
            myState[cx][cy].team=2;
        if(tree->child->isEmpty())
        {
            if(ischild)
                return;
            else
            {
                xx=x-1,yy=y-1;
                while(xx>=0&&yy>=0&&myState[xx][yy].team==0)
                {
                    methodTree *m=new methodTree(xx,yy);
                    tree->child->append(m);
                    xx--; yy--;
                }
                xx=x+1,yy=y-1;
                while(xx<=9&&yy>=0&&myState[xx][yy].team==0)
                {
                    methodTree *m=new methodTree(xx,yy);
                    tree->child->append(m);
                    xx++; yy--;
                }
                xx=x-1,yy=y+1;
                while(xx>=0&&yy<=9&&myState[xx][yy].team==0)
                {
                    methodTree *m=new methodTree(xx,yy);
                    tree->child->append(m);
                    xx--; yy++;
                }
                xx=x+1,yy=y+1;
                while(xx<=9&&yy<=9&&myState[xx][yy].team==0)
                {
                    methodTree *m=new methodTree(xx,yy);
                    tree->child->append(m);
                    xx++; yy++;
                }
            }
        }
    }
}

