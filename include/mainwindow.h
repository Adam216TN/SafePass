#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include "databasemanager.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class VaultWindow;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(DatabaseManager *dbManager,QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onUnlockClicked();
    void onLogout();

private:
    Ui::MainWindow *ui;
    VaultWindow *vault;
    QSystemTrayIcon *trayIcon;
    bool isUnlocked = false;
    QMenu *q;
    DatabaseManager *db;
};

#endif