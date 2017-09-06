#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QPainter>
#include <QLabel>
#include <QMouseEvent>
#include <game.h>
#include <QMessageBox>
#include "dlg.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
private:
    Ui::Widget *ui;
    QPushButton *giveup,*draw;
    QTextEdit *info;
    QLabel *infol;
    QPoint chosen;
    game *gm;
    dlg *dialog;
    bool yourturn=0;
signals:
    void trans(int,int,int,int);
    void gup(int);
private slots:
    void showMes(QString);
    void myturn();
    void finish();
    void givingup();
    void fail(int);
    void win(int);
    void nowin();
};

#endif // WIDGET_H
