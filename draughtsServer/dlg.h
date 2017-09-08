#ifndef DLG_H
#define DLG_H
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDesktopWidget>
#include <QApplication>
#include <QList>
#include <QHostAddress>
#include <QHostInfo>
#include <QPalette>
class dlg:public QDialog
{
    Q_OBJECT
public:
    dlg(QWidget *parent=0);
private:
    QPushButton *closing,*connecting;
    QLineEdit *ip,*port;
    QLabel *ipL,*portL;
private slots:
    void to_connect();
signals:
    void to_close();
    void connection(int);
};

#endif // DLG_H
