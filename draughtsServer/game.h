#ifndef GAME_H
#define GAME_H
#include <QObject>
#include <QList>
#include <QPoint>
#include <QTcpSocket>
#include <QTcpServer>
#include "methodtree.h"
#include <QtAlgorithms>
#include <QMessageBox>
#include <QtGlobal>
#include <QTime>
class state
{
public:
    int team;  //0没有棋子，1我方，2敌方，3暂时死亡
    bool isking=0;
};

class game:public QObject
{
    Q_OBJECT
private:
    state myState[10][10];
    state lastState[10][10];
    methodTree *mt[10][10];
    QTcpServer *server;
    QTcpSocket *socket;
    quint16 blocksize=0;
    void sendMes(QString);
    void getmethod();
    void trymethod(int x,int y,methodTree* tree,bool ischild,bool);
public:
    game();
    ~game();
    int chessman(int x,int y);
    bool isking(int x,int y);
    QList<QPoint> available(int x,int y);
private slots:
    void deal(int,int,int,int);
    void newConnect();
    void readMes();
    void beg();
    void giveup();
signals:
    void display();
    void finish();
    void defeat(int); //0 正常死亡 1 认输 2 无子可走
    void showMes(QString); //走棋格式 x1,y1,x2,y2
    void yourturn();
    void success(int);
    void accept();
    void connecting();
};
#endif // GAME_H
