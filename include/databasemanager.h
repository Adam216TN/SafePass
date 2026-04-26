#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include "simplecrypt.h"

struct Credential
{
    int id;
    QString username;
    QString password;
    QString website;
    QString url;
};

class DatabaseManager{

public:
    
    DatabaseManager();
    ~DatabaseManager();
    
    bool initDatabase();
    bool checkUserExists();

    bool addUser(const QString &email, const QString &password);
    bool addData(const QString &username, const QString &password, const QString &website, const QString &url);

    bool verifyLogin(const QString &password);

    QList<Credential> getAllCredentials();

private:
    QSqlDatabase db ;
    SimpleCrypt cryptor;

};

#endif