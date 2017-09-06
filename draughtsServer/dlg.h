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
class dlg:public QDialog
{
    Q_OBJECT
public:
    dlg(QWidget *parent=0);
private:
    QPushButton *closing;
    QLineEdit *ip,*port;
    QLabel *ipL,*portL;
signals:
    void to_close();
};

#endif // DLG_H
