#include "databasemanager.h"
#include <QDebug>
#include <QSqlError>

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

bool DatabaseManager::addUser(const QString &email, const QString &passwordHash){
    
    if(!db.open()){
        qDebug() << "Erreur d'ouverture : " << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    QString sql = "Insert into users(email, master_hash) Values ("
                  "?,?)";
            
    query.prepare(sql);
    query.addBindValue(email);
    query.addBindValue(passwordHash);

    return query.exec();
}