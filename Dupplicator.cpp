//
// Created by timpo on 3/23/19.
//

#include <QtCore/QCryptographicHash>
#include <QtCore/QDirIterator>
#include "Dupplicator.h"
#include <openssl/sha.h>


void Dupplicator::find(QString rootDir) {
    QVector<QVector<QString>> ans;
    try {
        QDirIterator it(rootDir, QDir::Hidden | QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        QVector<QPair<qint64, QString>> sizedFiles;
        while (it.hasNext()) {
            auto file = it.next();
            if (!it.fileInfo().isSymLink())
                sizedFiles.push_back(QPair<qint64, QString>(it.fileInfo().size(), file));
        }
        size_t total = sizedFiles.size();
//        emit progress(50, 100);
        std::sort(sizedFiles.begin(), sizedFiles.end(),
                  [](QPair<qint64, QString> &a, QPair<qint64, QString> &b) { return a.first > b.first; });
        int first_pointer = 0;
        while (first_pointer != sizedFiles.size()) {
            int second_pointer = first_pointer;
            while (second_pointer < sizedFiles.size() && sizedFiles[second_pointer].first == sizedFiles[first_pointer].first) {
                ++second_pointer;
            }
            if (second_pointer - first_pointer > 1) {
                QHash<QByteArray, QVector<QString>> hashToFiles;
                const int buffer_size = 1 << 18;
                char buffer[buffer_size];
                for (auto iter = sizedFiles.begin() + first_pointer; iter < sizedFiles.begin() + second_pointer; iter++) {
                    QFile filee(iter->second);
                    if (filee.open(QIODevice::ReadOnly | QIODevice::Text)) {
                        qint64 charRead = 0;
                        unsigned char temp_hash[SHA256_DIGEST_LENGTH];
                        char hash[SHA256_DIGEST_LENGTH];
                        SHA256_CTX sha;
                        SHA256_Init(&sha);
                        while ((charRead = filee.read(buffer, buffer_size)) > 0) {
                            SHA256_Update(&sha, buffer, static_cast<size_t>(charRead));
                        }
                        SHA256_Final(temp_hash, &sha);
                        convert_unsigned_to_signed(temp_hash, hash);
                        QByteArray localResult(hash, SHA256_DIGEST_LENGTH);
                        auto it = hashToFiles.find(localResult);
                        if (it != hashToFiles.end()) {
                            it->push_back(iter->second);
                        } else {
                            hashToFiles.insert(localResult, QVector<QString>({iter->second}));
                        }
                    }
                }

                for (auto &iter : hashToFiles) {
                    if (iter.size() > 1) {
                        ans.push_back(QVector<QString>());
                        for (auto &filePath : iter) {
                            ans[ans.size() - 1].push_back(filePath);
                        }
                    }
                }
            }
            first_pointer = second_pointer;
            emit progress(total, second_pointer);
        }
    } catch (...) {
        //no operations
    }
    emit searchResult(ans);
}


void Dupplicator::convert_unsigned_to_signed(const unsigned char *temp_ans,char *ans) const {
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
                            ans[i] = temp_ans[i];
                        }
}
