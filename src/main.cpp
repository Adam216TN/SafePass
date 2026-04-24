#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <mainwindow.h>
#include <registerwindow.h>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setApplicationName("SafePass");
    app.setApplicationDisplayName("SafePass");
    app.setQuitOnLastWindowClosed(false);
    
    RegisterWindow *window1 = new RegisterWindow();
    MainWindow window2;
    
    QObject::connect(window1,&RegisterWindow::createRequested,&window2,&MainWindow::show);
    
    window1->show();
    
    return app.exec();
}