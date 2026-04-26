#include "mainwindow.h"
#include "vaultwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QStyle>
#include <QAction>
#include <QApplication>
#include <QCursor>


MainWindow::MainWindow(DatabaseManager *dbManager,QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),db(dbManager),vault(nullptr), isUnlocked(false), q(new QMenu(this)){
    ui->setupUi(this);

    QAction *toggleAction = ui->lineEdit->addAction(QIcon(":/icons/assets/eye.png"),QLineEdit::TrailingPosition);
    
    connect(toggleAction,&QAction::triggered,[=](){
        if(ui->lineEdit->echoMode() == QLineEdit::Password){
            ui->lineEdit->setEchoMode(QLineEdit::Normal);
            toggleAction->setIcon(QIcon(":/icons/assets/closed_eye.png"));
        }else{
            ui->lineEdit->setEchoMode(QLineEdit::Password);
            toggleAction->setIcon(QIcon(":/icons/assets/eye.png"));
        }
    });
    connect(ui->btnconnect, &QPushButton::clicked, this, &MainWindow::onUnlockClicked);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::onUnlockClicked);
    
    ui->lineEdit->setFocus();
    this->setWindowTitle("SafePass");

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(style()->standardIcon(QStyle::SP_ComputerIcon));
    
    QAction *titleAction = q->addAction(style()->standardIcon(QStyle::SP_ComputerIcon),"SafePass");
    titleAction->setEnabled(false);
    
    q->addSeparator();
    
    QAction *openAction = q->addAction("Open SafePass");
    
    q->addSeparator();
    
    QAction *quitAction = q->addAction("Quit SafePass");
    
    connect(quitAction, &QAction::triggered,qApp, &QCoreApplication::quit);
    connect(openAction, &QAction::triggered, this, [this](){
    if(isUnlocked && vault) {
        vault->show();
        vault->raise();
        vault->activateWindow();
    } else {
        this->show();
        this->raise();
        this->activateWindow();
    }
    });

    trayIcon->show();
    
    trayIcon->setContextMenu(q);

    connect(trayIcon, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason){
    
        if(reason == QSystemTrayIcon::Trigger){
            if(isUnlocked && vault) {
                vault->show();
                vault->raise();
                vault->activateWindow();
            } else {
                this->show();
                this->raise();
                this->activateWindow();
            }
        } 
        
    });

}

MainWindow::~MainWindow(){
    delete ui;
    if(vault) delete vault;
}

void MainWindow::onUnlockClicked(){

    QString password = ui->lineEdit->text();

    if(db->verifyLogin(password)){

        //QMessageBox::information(this,"Allowed Access","Welcome to your password manager !");
        isUnlocked = true;
        if (vault==nullptr){
            vault = new VaultWindow(db);
            connect(vault, &VaultWindow::logoutRequested,this,&MainWindow::onLogout);
        }
        
        this->hide();
        ui->lineEdit->clear();
        vault->show();
    }else{

        QMessageBox::warning(this,"Acces Denied","The password is incorrect !");

        ui->lineEdit->clear();
    }

}

void MainWindow::onLogout(){
    isUnlocked = false;
    delete vault;
    vault = nullptr;
    this->show();
}