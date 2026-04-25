#include "databasemanager.h"
#include <QDebug>
#include <QSqlError>
#include <QCryptographicHash>

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
    QString sql = "CREATE TABLE IF NOT EXISTS users ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "email TEXT UNIQUE, "
                  "master_hash TEXT)";
    
    return query.exec(sql);
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
    QString sql = "Insert into users(email, master_hash) Values ("
                  "?,?)";

    QByteArray passwordData = password.toUtf8();
    QByteArray hashedData = QCryptographicHash::hash(passwordData,QCryptographicHash::Sha256);
    QByteArray passwordhash = hashedData.toHex();

    query.prepare(sql);
    query.addBindValue(email);
    query.addBindValue(passwordhash);

    return query.exec();
}

bool DatabaseManager::verifyLogin(const QString &password){
    
    QByteArray passwordData = password.toUtf8();
    QByteArray hashedData = QCryptographicHash::hash(passwordData,QCryptographicHash::Sha256);
    QByteArray passwordhash = hashedData.toHex();
    
    QSqlQuery query("Select master_hash from users");
    
    if(query.next() && passwordhash == query.value(0)){
        return true;
    }
    return false;

}