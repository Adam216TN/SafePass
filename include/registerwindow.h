#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class RegisterWindow;}
QT_END_NAMESPACE


class RegisterWindow : public QMainWindow{

    Q_OBJECT

public:
    RegisterWindow(QWidget *parent = nullptr);
    ~RegisterWindow();

private slots:

    void onCreateClick();

private:
    
    Ui::RegisterWindow *ui;

signals:

    void createRequested();

};


#endif