#ifndef DLG_H
#define DLG_H
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDesktopWidget>
#include <QApplication>
class dlg:public QDialog
{
    Q_OBJECT
public:
    dlg(QWidget *parent=0);
private:
    QPushButton *closing,*connecting;
    QLineEdit *ip,*port;
    QLabel *ipL,*portL;
signals:
    void to_close();
    void to_connect(QString,QString);
private slots:
    void connection();
};

#endif // DLG_H
