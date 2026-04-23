#ifndef VAULTWINDOW_H
#define VAULTWINDOW_H


#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class VaultWindow; }
QT_END_NAMESPACE


class VaultWindow : public QWidget {
    Q_OBJECT

public:
    VaultWindow(QWidget *parent = nullptr);
    ~VaultWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onDisconnectClicked();

private:
    Ui::VaultWindow *ui;

signals:
    void logoutRequested();
    
};


#endif