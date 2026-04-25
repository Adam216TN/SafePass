#ifndef VAULTWINDOW_H
#define VAULTWINDOW_H


#include <QWidget>
#include "databasemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class VaultWindow; }
QT_END_NAMESPACE


class VaultWindow : public QWidget {
    Q_OBJECT

public:
    VaultWindow(DatabaseManager *dbManager,QWidget *parent = nullptr);
    ~VaultWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onDisconnectClicked();
    void onAddClicked();
    void onRemoveClicked();

private:
    Ui::VaultWindow *ui;
    DatabaseManager *db;

signals:
    void logoutRequested();
    
};


#endif