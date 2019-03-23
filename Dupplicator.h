//
// Created by timpo on 3/23/19.
//

#ifndef DUPPLICATES_DUPPLICATOR_H
#define DUPPLICATES_DUPPLICATOR_H

#include <QtCore/QDir>
#include "mainwindow.h"

class Dupplicator : public QObject {
    Q_OBJECT

    friend class MainWindow;


public slots:
    void find(QString rootDir);

signals:
    void progress(qint64 processed, qint64 total);
    void searchResult(QVector<QVector<QString>>);

private:

    void convert_unsigned_to_signed(const unsigned char *temp_ans, char *ans) const;
};


#endif //DUPPLICATES_DUPPLICATOR_H
