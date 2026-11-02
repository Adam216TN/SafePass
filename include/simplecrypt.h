#ifndef SIMPLECRYPT_H
#define SIMPLECRYPT_H

#include <QString>

extern "C" {
    #include "aes.h"
}

class SimpleCrypt{

public:
    void setMasterKey(const QByteArray &password);
    void clear();
    QString encrypt(const QString &plainText);
    QString decrypt(const QString &cipherText);
    static QString password_generator(int n);

private:
    QByteArray m_key;
    inline static const QString CHARSET = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
};

#endif