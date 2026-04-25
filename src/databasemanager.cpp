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
    QString sqlUsers = "CREATE TABLE IF NOT EXISTS users ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "email TEXT UNIQUE, "
                  "master_hash TEXT)";
    
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


bool DatabaseManager::addData(const QString &username, const QString &password, const QString &website, const QString &url){

    QSqlQuery query;
    QString sql = "Insert into credentials(user_id, username, password, website, url) "
                  "Values(1,?,?,?,?)";

    query.prepare(sql);
    query.addBindValue(username);
    query.addBindValue(password);
    query.addBindValue(website);
    query.addBindValue(url);

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