#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class VaultWindow;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onUnlockClicked();
    void onTogglePassword(bool checked);
    void onLogout();

private:
    Ui::MainWindow *ui;
    VaultWindow *vault;
    QSystemTrayIcon *trayIcon;
    bool isUnlocked = false;
    QMenu *q;
};

#endif