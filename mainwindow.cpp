#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "offerer.h"
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
    QString ip = ui->plainTextEdit->toPlainText();
    offerer* offer = new offerer("mew");
    offer->runOfferer(ip.toStdString());
}


void MainWindow::on_pushButton_2_clicked()
{
    std::cout << "HI";
}


