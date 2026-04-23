#include "vaultwindow.h"
#include "ui_vaultwindow.h"
#include <QCloseEvent>

VaultWindow::VaultWindow(QWidget *parent) : QWidget(parent), ui(new Ui::VaultWindow){

    ui->setupUi(this);
    connect(ui->btndisc,&QPushButton::clicked,this,&VaultWindow::onDisconnectClicked);
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

