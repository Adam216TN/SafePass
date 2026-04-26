#include "registerwindow.h"
#include "ui_registerwindow.h"
#include <QRegularExpression>
#include <QMessageBox>


RegisterWindow::RegisterWindow(DatabaseManager *dbManager,QWidget *parent) : QMainWindow(parent), ui(new Ui::RegisterWindow), db(dbManager){
    ui->setupUi(this);

    QAction *toggleAction = ui->password->addAction(QIcon(":/icons/assets/eye.png"),QLineEdit::TrailingPosition);
    QAction *toggleAction2 = ui->confirm_password->addAction(QIcon(":/icons/assets/eye.png"),QLineEdit::TrailingPosition);
    
    connect(toggleAction, &QAction::triggered,[=](){
        if(ui->password->echoMode()==QLineEdit::Password){
            ui->password->setEchoMode(QLineEdit::Normal);
            toggleAction->setIcon(QIcon(":/icons/assets/closed_eye.png"));
        }else{
            ui->password->setEchoMode(QLineEdit::Password);
            toggleAction->setIcon(QIcon(":/icons/assets/eye.png"));
        }
    });

    connect(toggleAction2, &QAction::triggered,[=](){
        if(ui->confirm_password->echoMode()==QLineEdit::Password){
            ui->confirm_password->setEchoMode(QLineEdit::Normal);
            toggleAction2->setIcon(QIcon(":/icons/assets/closed_eye.png"));
        }else{
            ui->confirm_password->setEchoMode(QLineEdit::Password);
            toggleAction2->setIcon(QIcon(":/icons/assets/eye.png"));
        }
    });

    this->setFocus();
    this->setWindowTitle("SafePass");
    connect(ui->create, &QPushButton::clicked, this, &RegisterWindow::onCreateClick);
    connect(ui->confirm_password, &QLineEdit::returnPressed, this, &RegisterWindow::onCreateClick);

    this->setAttribute(Qt::WA_DeleteOnClose);
}

RegisterWindow::~RegisterWindow() {
    delete ui;
}

void RegisterWindow::onCreateClick(){
    QString email = ui->mail->text();
    QRegularExpression regex("^[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,}$", QRegularExpression::CaseInsensitiveOption);

    if(!regex.match(email).hasMatch()){
        QMessageBox::warning(this,"Error","The email address is not valid");
        return;
    }

    QString password = ui->password->text();
    QString confirm_password = ui->confirm_password->text();

    if(password != confirm_password || password.isEmpty()){
        QMessageBox::warning(this,"Error","The password is not the same");
        return;
    }

    db->addUser(email,password);

    emit createRequested();
    this->close();
}