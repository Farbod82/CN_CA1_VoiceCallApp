#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "offerer.h"
#include "answerer.h"
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

void MainWindow::on_pushButton_clicked()
{
    QString name = ui->plainTextEdit_friend_name->toPlainText();
    if (ui->radioButton_answerer->isChecked()){
        answerer ans(ui->plainTextEdit_name->toPlainText().toStdString());
        ans.runAnswerer();
    }
    else {
        offerer of(ui->plainTextEdit_name->toPlainText().toStdString(), ui->plainTextEdit_friend_name->toPlainText().toStdString());
        of.runOfferer();
    }
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

