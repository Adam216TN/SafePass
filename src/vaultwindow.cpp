#include "vaultwindow.h"
#include "ui_vaultwindow.h"
#include <QCloseEvent>
#include <QDialog>
#include <QFormLayout>
#include <QClipboard>
#include <QApplication>
#include <QToolButton>
#include <QMenu>

VaultWindow::VaultWindow( DatabaseManager *database, QWidget *parent) : QWidget(parent), ui(new Ui::VaultWindow),db(database){

    ui->setupUi(this);
    connect(ui->btndisc,&QPushButton::clicked,this,&VaultWindow::onDisconnectClicked);
    connect(ui->btnadd,&QPushButton::clicked,this,&VaultWindow::onAddClicked);
    this->setWindowTitle("SafePass");
    refreshTable();
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

    refreshTable();
}

void VaultWindow::onRemoveClicked(){}

void VaultWindow::refreshTable(){
    
    ui->vault->setRowCount(0);
    QList<Credential> liste = db->getAllCredentials();
    ui->vault->setRowCount(liste.length());
    
    for(int i = 0; i<liste.length();i++){
        
        ui->vault->setItem(i,0,new QTableWidgetItem(liste.at(i).username));
        QTableWidgetItem *password = new QTableWidgetItem(liste.at(i).password);
        password->setText("**********");
        password->setData(Qt::UserRole,liste.at(i).password);
        ui->vault->setItem(i,1,password);
        ui->vault->setItem(i,2,new QTableWidgetItem(liste.at(i).website));
        ui->vault->setItem(i,3,new QTableWidgetItem(liste.at(i).url));

        QToolButton *show = new QToolButton();
        show->setIcon(QIcon(":/icons/assets/eye.png"));
        
        connect(show,&QToolButton::clicked,[=](){
            if(password->text() == "**********"){
                show->setIcon(QIcon(":/icons/assets/closed_eye.png"));
                password->setText(password->data(Qt::UserRole).toString());
            }else{
                password->setText("**********");
                show->setIcon(QIcon(":/icons/assets/eye.png"));
            }
        });
        
        ui->vault->setCellWidget(i,4,show);

        QToolButton *copy = new QToolButton();
        connect(copy,&QToolButton::clicked,[=](){
            QMenu menu ;
            QAction *pass_copy = menu.addAction("Copy Password");
            QAction *user_copy = menu.addAction("Copy Username");

            connect(pass_copy, &QAction::triggered,[=](){
                QClipboard *clipboard = QApplication::clipboard();
                clipboard->setText(liste.at(i).password);
            });

            connect(user_copy,&QAction::triggered,[=](){
                QClipboard *clipboard = QApplication::clipboard();
                clipboard->setText(liste.at(i).username);
            });

            menu.exec(QCursor::pos());
        });

        copy->setIcon(QIcon(":/icons/assets/copy.png"));
        ui->vault->setCellWidget(i,5,copy);
        
    }
}