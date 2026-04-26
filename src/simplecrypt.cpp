#include "simplecrypt.h"
#include "QCryptographicHash"
#include "QRandomGenerator"

void SimpleCrypt::setMasterKey(const QString &password){
    
    QByteArray passwordData = password.toUtf8();
    m_key = QCryptographicHash::hash(passwordData,QCryptographicHash::Sha256);
}

QString SimpleCrypt::encrypt(const QString &plaintext){

    QByteArray data = plaintext.toUtf8();

    int blockSize = 16;
    int paddingSize = blockSize - (data.size() % blockSize);

    for(int i = 0; i <paddingSize; i++){
        data.append(static_cast<char>(paddingSize));
    }
    
    QByteArray iv(16,0);

    QRandomGenerator::securelySeeded().fillRange(reinterpret_cast<uint32_t*>(iv.data()),iv.size()/4);

    AES_ctx ctx;
    AES_init_ctx_iv(&ctx,reinterpret_cast<const uint8_t*>(m_key.constData()),reinterpret_cast<const uint8_t*>(iv.constData()));

    AES_CBC_encrypt_buffer(&ctx,reinterpret_cast<uint8_t*>(data.data()),data.size());

    QByteArray result;

    result.append(iv);
    result.append(data);

    return result.toBase64();
}

QString SimpleCrypt::decrypt(const QString &cipherText) {
    
    if (cipherText.isEmpty()) return "";
    
    QByteArray combined = QByteArray::fromBase64(cipherText.toUtf8());

    if (combined.size() < 16) return "";

    QByteArray iv = combined.left(16);
    
    QByteArray data = combined.mid(16);

    AES_ctx ctx;
    AES_init_ctx_iv(&ctx, 
                    reinterpret_cast<const uint8_t*>(m_key.constData()), 
                    reinterpret_cast<const uint8_t*>(iv.constData()));

    AES_CBC_decrypt_buffer(&ctx, 
                           reinterpret_cast<uint8_t*>(data.data()), 
                           data.size());

    if (data.isEmpty()) return "";
    
    int paddingVal = static_cast<unsigned char>(data.at(data.size() - 1));
    
    if (paddingVal > 0 && paddingVal <= 16 && paddingVal <= data.size()) {
        data.chop(paddingVal);
    }

    return QString::fromUtf8(data);
}