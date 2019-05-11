#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QLabel>
#include <QCloseEvent>
#include <QAbstractSocket>
#include <QHostAddress>
#include <QHostInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTcpSocket * tcpClient;//socket
    QLabel * LabSocketState;//状态栏显示标签
    QString getLocalIP();//获取本机IP地址
protected:
    void closeEvent(QCloseEvent * event);

private slots:
    void onConnected();
    void onDisconnected();
    void onSocketStateChange(QAbstractSocket::SocketState socketState);
    void onSocketReadyRead();//读取socket传入的数据

    void on_actConnect_clicked();
    void on_actDisconnect_clicked();
    void on_btnSend_clicked();
};

#endif // MAINWINDOW_H
