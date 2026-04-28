#ifndef VAULTWINDOW_H
#define VAULTWINDOW_H


#include <QWidget>
#include "databasemanager.h"
#include "simplecrypt.h"

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
    void onUpdateClicked();
    void refreshTable();
    void onSearch();

private:
    Ui::VaultWindow *ui;
    DatabaseManager *db;

signals:
    void logoutRequested();
    
};


#endif