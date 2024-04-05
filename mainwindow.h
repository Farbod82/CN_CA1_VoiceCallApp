#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_clicked();

    void on_radioButton_answerer_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    QThread* serverThread = nullptr;
    QThread* answererThread = nullptr;
    QThread* offererThread = nullptr;
};
#endif // MAINWINDOW_H
