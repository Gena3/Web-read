#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _isPageIsReading(false)
{

    ui->setupUi(this);
    _tmr = new QTimer(this);
    connect(_tmr, SIGNAL(timeout()), this, SLOT(updateTime()));
    connect(&_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onPageReadComplete(QNetworkReply*)));

    QSslConfiguration conf = _request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    _request.setSslConfiguration(conf);
}

MainWindow::~MainWindow()
{
    delete _tmr;
    delete ui;
}

bool MainWindow::IsStarted()
{
    return _tmr->isActive();
}


void MainWindow::updateTime()
{
    ui->statusbar->showMessage("Timer triggered");
    if(!_isPageIsReading){
        _isPageIsReading = true;
        _networkManager.get(_request);
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(IsStarted()){
        _tmr->stop();
        ui->pushButton->setText("Start");
        ui->statusbar->showMessage("Timer stoped");
    }else{
        int iTime;
        // set url
        _fileName = ui->lineEdit_4->text();
        _serviceUrl.setUrl(ui->lineEdit->text());
        _serviceUrl.setUserName(ui->lineEdit_2->text());
        _serviceUrl.setPassword(ui->lineEdit_3->text());
        _request.setUrl(_serviceUrl);
        // set timer
        iTime = ui->lineEdit_5->text().toInt();
        _tmr->setInterval((iTime < MIN_TIMEOUT ? MIN_TIMEOUT : iTime));
        _tmr->start();
        ui->pushButton->setText("Stop");
        ui->statusbar->showMessage("Timer started");
    }
}

void MainWindow::onPageReadComplete(QNetworkReply* reply)
{
    _isPageIsReading = false;
    if(reply->error() != QNetworkReply::NoError){
        ui->statusbar->showMessage("Error read page: " + reply->errorString());
    }else{
        QFile file(_fileName);
        file.open(QIODevice::WriteOnly);
        if(-1 == file.write(reply->readAll())){
            ui->statusbar->showMessage("Write file error");
        }else{
            ui->statusbar->showMessage("Write file success");
        }
        file.close();
    }
}
