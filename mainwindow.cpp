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

QString get_ip_address()
{
    QString ip_address;

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
                    ip_address = address.ip().toString();
                    break;
                }
            }

            if (!ip_address.isEmpty()) {
                break;
            }
        }
    }

    return ip_address;
}


void run_answerer(std::string name, QString ip) {
    answerer ans(name, ip);
    ans.run_answerer();
}
void run_offerer(std::string offerer_name, std::string answerer_name, QString ip) {
    offerer of(offerer_name, answerer_name, ip);
    of.run_offerer();
}

void run_server()
{
    TcpServer *s = new TcpServer();
    s->run_server2();
}

void MainWindow::on_pushButton_clicked()
{
    std::string name = ui->plainTextEdit_name->toPlainText().toStdString();
    QString server_ip = get_ip_address();
    if (ui->radioButton_answerer->isChecked()){
        qDebug() << "\nAnswerer ####";
        server_thread = new QThread();
        answerer_thread = new QThread();
        QObject::connect(server_thread, &QThread::started, []() { run_server(); });
        QObject::connect(answerer_thread, &QThread::started, [name, server_ip]() { run_answerer(name, server_ip); });
        server_thread->start();
        qDebug() << "\nServerRuned ####";
        answerer_thread->start();
    }
    else {
        qDebug() << "\nCaller ####";
        std::string friend_name = ui->plainTextEdit_friend_name->toPlainText().toStdString();
        // offerer_thread = new Q_thread();
        // QObject::connect(offerer_thread, &QThread::started, [name, server_ip, friend_name]() { run_offerer(name, friend_name, server_ip); });
        // offerer_thread->start();
        run_offerer(name, friend_name, server_ip);
    }
}

void MainWindow::on_radioButton_answerer_toggled(bool checked)
{
    if (checked){
        QString setting_text = "Your friend must enter this IP to connecto to you : \n" + get_ip_address();
        ui->label->setText(setting_text);
        ui->plainTextEdit_ip->hide();
        ui->plainTextEdit_friend_name->hide();
    }
    else{
        ui->label->setText("");
    }
}

