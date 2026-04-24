#include "registerwindow.h"
#include "ui_registerwindow.h"
#include <QRegularExpression>
#include <QMessageBox>


RegisterWindow::RegisterWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::RegisterWindow){
    ui->setupUi(this);

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

    emit createRequested();
    this->close();
}