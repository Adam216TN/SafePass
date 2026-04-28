#include "databasemanager.h"
#include <QDebug>
#include <QSqlError>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QPasswordDigestor>

DatabaseManager::DatabaseManager(){

    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("safepass.db");

}

DatabaseManager::~DatabaseManager(){
    if(db.isOpen()){
        db.close();
    }
}

bool DatabaseManager::initDatabase(){
    if(!db.open()){
        qDebug() << "Erreur d'ouverture : " << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    QString sqlUsers = "CREATE TABLE IF NOT EXISTS users ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "email TEXT UNIQUE, "
                  "master_hash TEXT, "
                  "salt TEXT)";
    
    if(!query.exec(sqlUsers)) return false;

    QString sqlData = "Create Table IF NOT EXISTS credentials("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "user_id INTEGER,"
                      "username TEXT, "
                      "password TEXT, "
                      "website TEXT, "
                      "url TEXT,"
                      "FOREIGN KEY(user_id) REFERENCES users(id))";

    return query.exec(sqlData);
}

bool DatabaseManager::checkUserExists() {
    QSqlQuery query("SELECT COUNT(*) FROM users");
    if (query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

bool DatabaseManager::addUser(const QString &email, const QString &password){
    
    if(!db.open()){
        qDebug() << "Erreur d'ouverture : " << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    QString sql = "Insert into users(email, master_hash, salt) Values ("
                  "?,?,?)";

    QByteArray passwordData = password.toUtf8();
    
    QByteArray salt(16,0);
    QRandomGenerator::securelySeeded().fillRange(reinterpret_cast<uint32_t*>(salt.data()),salt.size()/4);
    QByteArray hashedData = QPasswordDigestor::deriveKeyPbkdf2(QCryptographicHash::Sha256,passwordData,salt,100000,32);
    
    QByteArray passwordhash = hashedData.toHex();

    query.prepare(sql);
    query.addBindValue(email);
    query.addBindValue(passwordhash);
    query.addBindValue(salt.toHex());

    return query.exec();
}


bool DatabaseManager::addData(const QString &username, const QString &password, const QString &website, const QString &url){

    QString encryptedPassword = cryptor.encrypt(password);
    
    QSqlQuery query;
    QString sql = "Insert into credentials(user_id, username, password, website, url) "
                  "Values(1,?,?,?,?)";

    query.prepare(sql);
    query.addBindValue(username);
    query.addBindValue(encryptedPassword);
    query.addBindValue(website);
    query.addBindValue(url);

    return query.exec();
}

bool DatabaseManager::removeData(int id){

    QSqlQuery query;
    QString sql = "Delete From credentials"
                  " Where id = ?";
    
    query.prepare(sql);
    query.addBindValue(id);

    return query.exec();
}

bool DatabaseManager::updateData(int id,const QString &username, const QString &password, const QString &website, const QString &url){
    
    QString encryptedPassword = cryptor.encrypt(password);
    
    QSqlQuery query;
    QString sql = "Update credentials "
                  "Set username = ?, password = ?, website = ?, url = ?"
                  " Where id = ?";
    
    query.prepare(sql);
    query.addBindValue(username);
    query.addBindValue(encryptedPassword);
    query.addBindValue(website);
    query.addBindValue(url);
    query.addBindValue(id);

    return query.exec();
}

bool DatabaseManager::verifyLogin(const QString &password){
    
    QSqlQuery query("Select salt, master_hash from users");

    if(query.next()){

        QByteArray s = query.value(0).toByteArray();
        QByteArray salt = QByteArray::fromHex(s);
        QByteArray passwordData = password.toUtf8();
        QByteArray hashedData = QPasswordDigestor::deriveKeyPbkdf2(QCryptographicHash::Sha256,passwordData,salt,100000,32);
        
        QByteArray passwordhash = hashedData.toHex();
        
        if(passwordhash == query.value(1)){
            cryptor.setMasterKey(hashedData);
            return true;
        }
    }
    
    return false;
}

QList<Credential> DatabaseManager::getAllCredentials(){

    QList<Credential> liste;
    
    QSqlQuery query;
    QString sql = "Select id, username, password, website, url from credentials";

    query.exec(sql);

    while(query.next()){
        Credential c;
        c.id = query.value(0).toInt();
        c.username = query.value(1).toString();
        QString decrypted_password = cryptor.decrypt(query.value(2).toString());
        c.password = decrypted_password;
        c.website = query.value(3).toString();
        c.url = query.value(4).toString();

        liste.append(c);
    }

    return liste;

}