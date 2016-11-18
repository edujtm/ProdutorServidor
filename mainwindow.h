#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    bool started = false;
    QTimer *timer;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void tcpConnect();
    void tcpDisconnect();
    bool isStarted();
public slots:
    void putData();
private slots:
    void on_pushButtonConnect_clicked();


    void on_pushButtonDisconnect_clicked();

    void on_pushButtonPut_clicked();

    void on_pushButtonStop_clicked();

    void on_horizontalSliderTiming_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
};

#endif // MAINWINDOW_H
