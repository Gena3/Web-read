#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define MIN_TIMEOUT 1000

#include <QMainWindow>
#include <QTimer>
#include <QUrl>
#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkreply.h>
#include <qfile.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow          *ui;
    QTimer                  *_tmr;
    QString                 _link, _user, _pass, _fileName;
    QUrl                    _serviceUrl;
    bool                    _isPageIsReading;
    QNetworkAccessManager   _networkManager;
    QNetworkRequest         _request;

    bool            IsStarted(void);

private slots:
    void updateTime();
    void on_pushButton_clicked();
    void onPageReadComplete(QNetworkReply* reply);
};
#endif // MAINWINDOW_H
