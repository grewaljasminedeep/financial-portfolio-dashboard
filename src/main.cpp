#include "MainWindow.h"
#include "Config.h"
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QString configPath = QDir::currentPath() + "/config.yaml";
    if (!QFile::exists(configPath)) {
        qWarning() << "config.yaml not found at" << configPath << "; using defaults.";
    }

    AppConfig cfg;
    try {
        cfg = loadConfig(configPath.toStdString());
    } catch (const std::exception& e) {
        qWarning() << "Failed to load config:" << e.what() << "; using defaults.";
        // cfg remains default-constructed
    }

    MainWindow w(cfg);
    w.resize(1100, 650);
    w.show();
    return app.exec();
}