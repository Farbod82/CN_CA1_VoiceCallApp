#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "offerer.h"
#include "answerer.h"
#include "signaling_server.h"
#include <QtNetwork>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString getIpAddress()
{
    QString ipAddress;

    // Get list of all network interfaces on the device
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    // Iterate over each network interface to find the one that is up and is not a loopback
    for (const QNetworkInterface &interface : interfaces) {
        if (interface.isValid() && interface.flags().testFlag(QNetworkInterface::IsUp)
            && !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {

            // Get list of all IP addresses associated with this interface
            QList<QNetworkAddressEntry> addresses = interface.addressEntries();
            for (const QNetworkAddressEntry &address : addresses) {

                // Check if the IP address is IPv4 and not a link-local address
                if (address.ip().protocol() == QAbstractSocket::IPv4Protocol
                    && !address.ip().isLinkLocal()) {
                    ipAddress = address.ip().toString();
                    break;
                }
            }

            if (!ipAddress.isEmpty()) {
                break;
            }
        }
    }

    return ipAddress;
}


void runAnswerer(std::string name, QString ip) {
    answerer ans(name, ip);
    ans.runAnswerer();
}
void runOfferer(std::string offerer_name, std::string answerer_name, QString ip) {
    offerer of(offerer_name, answerer_name, ip);
    of.runOfferer();
}

void runServer()
{
    TcpServer *s = new TcpServer();
    s->runServer2();
}

void MainWindow::on_pushButton_clicked()
{
    std::string name = ui->plainTextEdit_name->toPlainText().toStdString();
    QString server_ip = getIpAddress();
    if (ui->radioButton_answerer->isChecked()){
        qDebug() << "\nAnswerer ####";
        serverThread = new QThread();
        answererThread = new QThread();
        QObject::connect(serverThread, &QThread::started, []() { runServer(); });
        QObject::connect(answererThread, &QThread::started, [name, server_ip]() { runAnswerer(name, server_ip); });
        serverThread->start();
        qDebug() << "\nServerRuned ####";
        answererThread->start();
    }
    else {
        qDebug() << "\nCaller ####";
        std::string friend_name = ui->plainTextEdit_friend_name->toPlainText().toStdString();
        // offererThread = new QThread();
        // QObject::connect(offererThread, &QThread::started, [name, server_ip, friend_name]() { runOfferer(name, friend_name, server_ip); });
        // offererThread->start();
        runOfferer(name, friend_name, server_ip);
    }
}

void MainWindow::on_radioButton_answerer_toggled(bool checked)
{
    if (checked){
        QString setting_text = "Your friend must enter this IP to connecto to you : \n" + getIpAddress();
        ui->label->setText(setting_text);
        ui->plainTextEdit_ip->hide();
        ui->plainTextEdit_friend_name->hide();
    }
    else{
        ui->label->setText("");
    }
}

