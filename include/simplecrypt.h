#ifndef SIMPLECRYPT_H
#define SIMPLECRYPT_H

#include <QString>

extern "C" {
    #include "aes.h"
}

class SimpleCrypt{

public:
    void setMasterKey(const QString &password);

    QString encrypt(const QString &plainText);
    QString decrypt(const QString &cipherText);

private:
    QByteArray m_key;

};




#endif