#include "vaultwindow.h"
#include "ui_vaultwindow.h"
#include <QCloseEvent>
#include <QDialog>
#include <QFormLayout>

VaultWindow::VaultWindow( DatabaseManager *database, QWidget *parent) : QWidget(parent), ui(new Ui::VaultWindow),db(database){

    ui->setupUi(this);
    connect(ui->btndisc,&QPushButton::clicked,this,&VaultWindow::onDisconnectClicked);
    connect(ui->btnadd,&QPushButton::clicked,this,&VaultWindow::onAddClicked);
    this->setWindowTitle("SafePass");
}

VaultWindow::~VaultWindow(){
    delete ui;
}

void VaultWindow::onDisconnectClicked(){
    emit logoutRequested();
}

void VaultWindow::closeEvent(QCloseEvent *event){
    event->ignore();
    this->hide();
}

void VaultWindow::onAddClicked(){
    
    QDialog qd(this);
    QVBoxLayout qv(&qd);
    QFormLayout l;
    
    QLineEdit *username = new QLineEdit(&qd);
    QLineEdit *password = new QLineEdit(&qd);
    QLineEdit *website = new QLineEdit(&qd);
    QLineEdit *url = new QLineEdit(&qd);
    QPushButton *accept = new QPushButton("Accept",&qd);
    
    l.addRow("Username",username);
    l.addRow("Password",password);
    l.addRow("Website",website);
    l.addRow("URL",url);
    
    qv.addLayout(&l);
    qv.addWidget(accept);
    
    connect(accept,&QPushButton::clicked,&qd,&QDialog::accept);
    
    if(qd.exec() == QDialog::Accepted){

        QString user = username->text();
        QString pass = password->text();
        QString web = website->text();
        QString link = url->text();

        db->addData(user,pass,web,link);
    }
}

void VaultWindow::onRemoveClicked(){}