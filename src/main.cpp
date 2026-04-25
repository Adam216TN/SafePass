#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <mainwindow.h>
#include <registerwindow.h>
#include "databasemanager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    DatabaseManager db;
    db.initDatabase();

    app.setApplicationName("SafePass");
    app.setApplicationDisplayName("SafePass");
    app.setQuitOnLastWindowClosed(false);
    
    MainWindow window2(&db);
    if(!db.checkUserExists()){
        RegisterWindow *window1 = new RegisterWindow(&db);
        window1->show();
        QObject::connect(window1,&RegisterWindow::createRequested,&window2,&MainWindow::show);
    }else{
        window2.show();
    }
    
    return app.exec();
}