#include "widget.h"
#include "./ui_widget.h"

//-------------------------------------------------------------------------------------------
Widget::Widget(QWidget *parent)
    :
    QWidget(parent),
    ui(new Ui::Widget),
    PORT(0),
    IP_ADDRESS(""),
    client(nullptr)
{
    ui->setupUi(this);
   // ui->label->setFixedWidth(150);
    this->setWindowTitle("Chattt");
    // Connect button signals to slots
    //connect(client, &NetChat::Client::messageReceived, this, &Widget::on_messageReceived);
    ui->nameLED->setPlaceholderText("Enter your userName...");
    ui->inputTE->setPlaceholderText("Message...");
    ui->portLED->setPlaceholderText("Enter the server PORT...");
    ui->ipAddressLED->setPlaceholderText("Enter the server IP_ADDRESS...");
    ui->sendPBN->setEnabled(false);
    ui->inputTE->setEnabled(false);
    log( "The application is running!" );

    ui->inputTE->installEventFilter(this);   // for keyboard

}
//-------------------------------------------------------------------------------------------
Widget::~Widget()
{
    delete ui;
    if (client){ delete client;}
}
//-------------------------------------------------------------------------------------------
void Widget::on_connectPBN_clicked()
{
    userName  = ui->nameLED->text();

    if (userName.isEmpty() ||  ui->portLED->text().isEmpty() || ui->ipAddressLED->text().isEmpty()){
        log("Please enter username, ip address and port!");
    } else {
        PORT = std::stoi(ui->portLED->text().toStdString());
        IP_ADDRESS = ui->ipAddressLED->text().toStdString();
        client = new NetChat::Client(PORT, IP_ADDRESS);
        connect(client, &NetChat::Client::messageReceived, this, &Widget::on_messageReceived); // connect signal slot!
        connect(client, &NetChat::Client::disconnectServer, this, &Widget::on_disconnectServer);
        try {
            client->connect();      // connect client
            std::string pport = std::to_string(PORT);
            log("CONNECTED TO SERVER AT: " + QString::fromStdString(IP_ADDRESS) +":"+ QString::fromStdString(ui->portLED->text().toStdString()));
            ui->connectPBN->setEnabled(false);
            ui->inputTE->setEnabled(true);
            ui->portLED->setEnabled(false);
            ui->ipAddressLED->setEnabled(false);
            ui->sendPBN->setEnabled(true);
        }catch(const std::exception& e) {
            log(QString("Connection failed...: ") + e.what());
            delete client;
            client = nullptr;
        }
    }
}
//-------------------------------------------------------------------------------------------
void Widget::on_sendPBN_clicked()
{
    if(!client){
        log("The client has not connected to the server");
    } else if (!ui->inputTE->toPlainText().isEmpty()){
        QString message = "[" + userName + "]: ";
        message +=ui->inputTE->toPlainText();
        client->sendMessage(message.toStdString());
        log(message);
        ui->inputTE->clear();
    }
}
//-------------------------------------------------------------------------------------------

void Widget::log(const QString &s)
{
     QString m;
     m = QTime::currentTime().toString("hh:mm");
     m += " - ";
     m += s;
     ui->logTE->append(m);
    // qDebug() << m;
}
//-------------------------------------------------------------------------------------------
void Widget::on_messageReceived(const QString &message)
{
    log(message);
}
//-------------------------------------------------------------------------------------------
void Widget::on_disconnectServer(){
    ui->connectPBN->setEnabled(true);
    log("The server crashed. Click the connect button again!");
}
//-------------------------------------------------------------------------------------------
bool Widget::eventFilter(QObject *obj, QEvent *event) {
    // проверяем такое ли событие вообще
    if (obj == ui->inputTE && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

        // Проверяем, была ли нажата клавиша Enter (без Shift)
        if ((keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) &&
            !(keyEvent->modifiers() & Qt::ControlModifier)) {
            on_sendPBN_clicked(); // Вызываем метод отправки сообщения
            return true; // Событие обработано
        }

        if ((keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) &&
            (keyEvent->modifiers() & Qt::ControlModifier)) {
            // Вставляем перенос строки
            ui->inputTE->insertPlainText("\n");
            return true; // Событие обработано
        }
    }
    return QWidget::eventFilter(obj, event);
}
