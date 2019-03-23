#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void handleStartButton();
    void handleDeleteButton();
    void handleProgress(qint64 processed, qint64 total);
    void showResults(QVector<QVector<QString>>result);
signals:
    void startFind(QString rootPath);

private:

    Ui::MainWindow *ui;
    QString rootPath;
};

#endif // MAINWINDOW_H
