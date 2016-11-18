#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  socket = new QTcpSocket(this);
  tcpConnect();

}

void MainWindow::tcpConnect(){
  socket->connectToHost(ui->lineEdit->text(),1234);
  if(socket->waitForConnected(3000)){
    qDebug() << "Connected";
  }
  else{
    qDebug() << "Disconnected";
  }
}

void MainWindow::tcpDisconnect()
{
    socket->disconnectFromHost();
    if(socket->state() == QAbstractSocket::UnconnectedState || socket->waitForDisconnected(3000)){
        if(isStarted())
        {
            timer->stop();
            started = false;
        }
        qDebug() << "Disconnected";
    } else {
        qDebug() << "Connected";
    }
}

void MainWindow::putData()
{
  int min, max;
  min = ui->lcdMin->intValue();
  max = ui->lcdMax->intValue();
  if(max > min){
      QDateTime datetime;
      QString str;
      if(socket->state()== QAbstractSocket::ConnectedState){
          datetime = QDateTime::currentDateTime();
          str = "set "+
              datetime.toString(Qt::ISODate)+
              " "+
              QString::number( min+(qrand()%(max-min)))+"\r\n";
          ui->DisplayData->append(str);
          qDebug() << str;
          qDebug() << socket->write(str.toStdString().c_str()) << " bytes written";
          if(socket->waitForBytesWritten(3000)){
            qDebug() << "wrote";
          }
      }
  }
  else
  {
      QString str = "Min value greater than Max value.\n";
      ui->DisplayData->append(str);
  }
}

MainWindow::~MainWindow()
{
  delete socket;
  delete ui;
}

void MainWindow::on_pushButtonConnect_clicked()
{
    MainWindow::tcpConnect();
}


void MainWindow::on_pushButtonDisconnect_clicked()
{
    MainWindow::tcpDisconnect();
}

void MainWindow::on_pushButtonPut_clicked()
{
    if(socket->state() == QAbstractSocket::ConnectedState && !isStarted())
    {
        timer = new QTimer(this);
        connect(timer,
                SIGNAL(timeout()),
                this,
                SLOT(putData()));
        timer->start(1000);
        started = true;
    }
}

void MainWindow::on_pushButtonStop_clicked()
{
    if(isStarted())
    {
        timer->stop();
        started = false;
    }
}

bool MainWindow::isStarted()
{
        return started;
}

void MainWindow::on_horizontalSliderTiming_valueChanged(int value)
{
    if(isStarted())
    {
        timer->setInterval(value*100);
    }
    ui->labelTiming->setNum((double) value/10);
}
