#ifndef WIDGET_H
#define WIDGET_H
//------------------------------------------------------------------------------------------
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QTime>
#include <QGridLayout>
#include <QKeyEvent>  // for enter
#include "clientChat.h"
//-------------------------------------------------------------------------------------------



QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

namespace NetChat {
    class Client;
}


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:


    void on_connectPBN_clicked();

    void on_messageReceived(const QString& message);
    void on_disconnectServer();

    void on_sendPBN_clicked();



private:
    Ui::Widget *ui;
    QString              userName;
    int                      PORT;
    std::string IP_ADDRESS;
    NetChat::Client*       client;

    void    log(const QString &s);

};
#endif // WIDGET_H
