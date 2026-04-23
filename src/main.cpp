#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <mainwindow.h>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setApplicationName("SafePass");
    app.setApplicationDisplayName("SafePass");
    app.setQuitOnLastWindowClosed(false);
    
    MainWindow window;

    window.show();
    return app.exec();
}