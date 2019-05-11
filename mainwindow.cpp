#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpClient = new QTcpSocket(this);//创建Socket变量
    LabSocketState = new QLabel("Socket状态: ");//状态栏标签
    LabSocketState->setMinimumWidth(250);
    this->ui->statusBar->addWidget(LabSocketState);
    QString localIP = getLocalIP();//本机IP
    this->setWindowTitle(this->windowTitle() + "-----本机IP:" + localIP);
    this->ui->comboServer->addItem(localIP);

    connect(tcpClient, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(tcpClient, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(tcpClient, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChange(QAbstractSocket::SocketState)));
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));


}

void MainWindow::onDisconnected()
{
    //客户端断开连接时
    this->ui->plainTextEdit->appendPlainText("**client socket disconnected");
    tcpClient->deleteLater();
}

void MainWindow::onConnected()
{
    //连上服务器时
    this->ui->plainTextEdit->appendPlainText("**已连接到服务器");
    this->ui->plainTextEdit->appendPlainText("**peer address:" + tcpClient->peerAddress().toString());
    this->ui->plainTextEdit->appendPlainText("**peer port：" + QString::number(tcpClient->peerPort()));
}




QString MainWindow::getLocalIP()
{
    //获取本机IPv4地址
    QString hostName = QHostInfo::localHostName();//本地主机名
    QHostInfo hostInfo  = QHostInfo::fromName(hostName);
    QString localIP = "";
    QList<QHostAddress> addList = hostInfo.addresses();

    if(!addList.isEmpty()){
        for(int i = 0; i < addList.count(); i++){
            QHostAddress aHost = addList.at(i);
            if(QAbstractSocket::IPv4Protocol == aHost.protocol()){
                localIP = aHost.toString();
                break;
            }
        }
    }
    return localIP;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actConnect_clicked()
{
    //"连接到服务器"
    QString addr = this->ui->comboServer->currentText();
    quint16 port = this->ui->spinPort->value();
    tcpClient->connectToHost(addr, port);
}


void MainWindow::on_actDisconnect_clicked()
{
    //"断开连接"
    this->ui->plainTextEdit->appendPlainText("**已连接到服务器");
    this->ui->plainTextEdit->appendPlainText("**peer address:" + tcpClient->peerAddress().toString());
    this->ui->plainTextEdit->appendPlainText("**peer port:" + QString::number(tcpClient->peerPort()));
    this->ui->actConnect->setEnabled(true);
    this->ui->actDisconnect->setEnabled(false);
}

void MainWindow::onSocketStateChange(QAbstractSocket::SocketState socketState)
{
    //socket状态发生变化时
    switch(socketState){
    case QAbstractSocket::UnconnectedState:
        LabSocketState->setText("socket状态: UnconnectedState"); break;
    case QAbstractSocket::HostLookupState:
        LabSocketState->setText("socke状态: HostLookupState"); break;
    case QAbstractSocket::ConnectingState:
        LabSocketState->setText("socket状态: ConnectingState"); break;
    case QAbstractSocket::ConnectedState:
        LabSocketState->setText("socket状态: ConnectedState"); break;
    case QAbstractSocket::BoundState:
        LabSocketState->setText("socket状态: BoundState"); break;
    case QAbstractSocket::ClosingState:
        LabSocketState->setText("socket状态：ClosingState"); break;
    case QAbstractSocket::ListeningState:
        LabSocketState->setText("socket状态: ListeningState");
    }
}



void MainWindow::on_btnSend_clicked()
{
    //发送数据
   QString msg = this->ui->editMsg->text();
   this->ui->plainTextEdit->appendPlainText("[out]" + msg);
   this->ui->editMsg->clear();
   this->ui->editMsg->setFocus();
   QByteArray str = msg.toUtf8();
   str.append('\n');
   tcpClient->write(str);
}

void MainWindow::onSocketReadyRead()
{
    //readyRead()信号槽函数
    while(tcpClient->canReadLine()){
        this->ui->plainTextEdit->appendPlainText("[in] " + tcpClient->readLine());
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //delete tcpClient;
    delete  LabSocketState;
    QMainWindow::closeEvent(event);
}
