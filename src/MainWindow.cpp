#include "MainWindow.h"
#include "PortfolioEngine.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QLabel>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), db_({ "localhost", 5432, "portfolio_db", "trader", "secure_pass" }) {
    setWindowTitle("Financial Portfolio Dashboard");
}