#include "vaultwindow.h"
#include "ui_vaultwindow.h"
#include <QCloseEvent>
#include <QDialog>
#include <QFormLayout>
#include <QClipboard>
#include <QApplication>
#include <QToolButton>
#include <QMenu>
#include <QMessageBox>


VaultWindow::VaultWindow( DatabaseManager *database, QWidget *parent) : QWidget(parent), ui(new Ui::VaultWindow),db(database){

    ui->setupUi(this);
    connect(ui->btndisc,&QPushButton::clicked,this,&VaultWindow::onDisconnectClicked);
    connect(ui->btnadd,&QPushButton::clicked,this,&VaultWindow::onAddClicked);
    connect(ui->btnrem,&QPushButton::clicked,this,&VaultWindow::onRemoveClicked);
    connect(ui->btnupd,&QPushButton::clicked,this,&VaultWindow::onUpdateClicked);
    this->setWindowTitle("SafePass");
    refreshTable();
    ui->vault->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->vault->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->vault->setSelectionBehavior(QAbstractItemView::SelectRows);
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
    
    QAction *generate = password->addAction(style()->standardIcon(QStyle::SP_BrowserReload),QLineEdit::TrailingPosition);
    generate->setToolTip("Generate");

    QSlider *slider = new QSlider(Qt::Horizontal, &qd);
    slider->setSingleStep(1);
    slider->setTickInterval(3);
    slider->setMinimum(8);
    slider->setMaximum(32);
    slider->setValue(16);
    slider->setTickPosition(QSlider::TicksBelow);

    QLabel *label = new QLabel(&qd);
    label->setText(QString::number(slider->value()));
    
    connect(generate,&QAction::triggered,[=](){
        QString s = SimpleCrypt::password_generator(slider->value());
        password->setText(s);
    });

    connect(slider,&QSlider::valueChanged,[=](int val){
        label->setText(QString::number(val));
    });
    
    
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(slider);
    hLayout->addWidget(label);
    
    l.addRow("Username",username);
    l.addRow("Password",password);
    l.addRow(hLayout);
    l.addRow("Website",website);
    l.addRow("URL",url);
    
    qv.addLayout(&l);
    qv.addWidget(accept);

    qd.resize(400,400);
    
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

void VaultWindow::onRemoveClicked(){

    int row = ui->vault->currentRow();
    if(row == -1) return;
    QTableWidgetItem *elt = ui->vault->item(row,0);
    int id = elt->data(Qt::UserRole).toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Deletion", 
                                     "Are you sure you want to delete this credential?",
                                     QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes){
        if(db->removeData(id)){
            refreshTable();
        }
    }
}

void VaultWindow::onUpdateClicked(){

    int row = ui->vault->currentRow();
    if(row == -1) return;

    int id = ui->vault->item(row,0)->data(Qt::UserRole).toInt();

    QString us = ui->vault->item(row,0)->text();
    QString pw = ui->vault->item(row,1)->data(Qt::UserRole).toString();
    QString ws = ui->vault->item(row,2)->text();
    QString ul = ui->vault->item(row,3)->text();
    
    QDialog qd(this);
    QVBoxLayout qv(&qd);
    QFormLayout l;

    QLineEdit *username = new QLineEdit(&qd);
    QLineEdit *password = new QLineEdit(&qd);
    QLineEdit *website = new QLineEdit(&qd);
    QLineEdit *url = new QLineEdit(&qd);
    QPushButton *update = new QPushButton("Update",&qd);

    username->setText(us);
    password->setText(pw);
    website->setText(ws);
    url->setText(ul);

    QAction *generate = password->addAction(style()->standardIcon(QStyle::SP_BrowserReload),QLineEdit::TrailingPosition);
    generate->setToolTip("Generate");

    QSlider *slider = new QSlider(Qt::Horizontal, &qd);
    slider->setSingleStep(1);
    slider->setTickInterval(3);
    slider->setMinimum(8);
    slider->setMaximum(32);
    slider->setValue(16);
    slider->setTickPosition(QSlider::TicksBelow);

    QLabel *label = new QLabel(&qd);
    label->setText(QString::number(slider->value()));
    
    connect(generate,&QAction::triggered,[=](){
        QString s = SimpleCrypt::password_generator(slider->value());
        password->setText(s);
    });

    connect(slider,&QSlider::valueChanged,[=](int val){
        label->setText(QString::number(val));
    });
    
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(slider);
    hLayout->addWidget(label);
    
    l.addRow("Username",username);
    l.addRow("Password",password);
    l.addRow(hLayout);
    l.addRow("Website",website);
    l.addRow("URL",url);
    
    qv.addLayout(&l);
    qv.addWidget(update);

    qd.resize(400,400);
    
    connect(update,&QPushButton::clicked,&qd,&QDialog::accept);

    if(qd.exec() == QDialog::Accepted){

        QString user = username->text();
        QString pass = password->text();
        QString web = website->text();
        QString link = url->text();

        db->updateData(id,user,pass,web,link);
    }

    refreshTable();

}

void VaultWindow::refreshTable(){
    
    ui->vault->setRowCount(0);
    QList<Credential> liste = db->getAllCredentials();
    ui->vault->setRowCount(liste.length());
    
    for(int i = 0; i<liste.length();i++){
        
        QTableWidgetItem *username = new QTableWidgetItem(liste.at(i).username);
        username->setData(Qt::UserRole,liste.at(i).id);
        ui->vault->setItem(i,0,username);
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