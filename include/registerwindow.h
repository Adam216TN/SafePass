#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QMainWindow>
#include "databasemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RegisterWindow;}
QT_END_NAMESPACE


class RegisterWindow : public QMainWindow{

    Q_OBJECT

public:
    RegisterWindow(DatabaseManager *dbManager, QWidget *parent = nullptr);
    ~RegisterWindow();

private slots:

    void onCreateClick();

private:
    
    Ui::RegisterWindow *ui;
    DatabaseManager *db;

signals:

    void createRequested();

};


#endif