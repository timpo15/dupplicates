#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Dupplicator.h"

#include <QCommonStyle>
#include <QDesktopWidget>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    auto *dupplicator = new Dupplicator;
    ui->setupUi(this);

//    qRegisterMetaType<QVector<QVector<QString>>>("QVector<QVector<QString>>");

    connect(this, &MainWindow::startFind, dupplicator, &Dupplicator::find);
    connect(dupplicator, SIGNAL(searchResult(QVector<QVector<QString>>)), this,
            SLOT(showResults(QVector<QVector<QString>>)));
    connect(dupplicator, &Dupplicator::progress, this, &MainWindow::handleProgress);

    connect(ui->startButton, SIGNAL(released()), this, SLOT(handleStartButton()));
    connect(ui->deleteButton, SIGNAL(released()), this, SLOT(handleDeleteButton()));

}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::handleStartButton() {
    rootPath = QFileDialog::getExistingDirectory(this, "Select Directory", QString(),
                                                 QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->progressBar->setValue(0);
    ui->statusLabel->setText(QString("finding dupplicates"));
    if (!rootPath.isEmpty()) {
        ui->treeWidget->clear();
        emit startFind(rootPath);
    }
}

void MainWindow::handleProgress(qint64 processed, qint64 total) {
    ui->progressBar->setValue(int(processed * 100 / total));
}

void MainWindow::handleDeleteButton() {
    auto selected = ui->treeWidget->selectedItems();
    for (auto &item : selected) {
        if (item->parent() != nullptr) {
            if (QFile::remove(item->text(0)))
                delete item;
        }
    }
}
    void MainWindow::showResults(QVector<QVector<QString>> results) {
        ui->statusLabel->setText(QString("finished"));
        for (auto result : results) {
            auto *item = new QTreeWidgetItem(ui->treeWidget);
            ui->treeWidget->addTopLevelItem(item);
            for (auto &filePath : result) {
                QTreeWidgetItem *itemChild = new QTreeWidgetItem(item);
                itemChild->setText(0, filePath);
                item->addChild(itemChild);
            }
            item->setText(0, QString::number(result.size()).append(" files"));
            item->setExpanded(false);
        }
    }