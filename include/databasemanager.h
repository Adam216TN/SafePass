#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>

class DatabaseManager{

public:
    
    DatabaseManager();
    ~DatabaseManager();
    
    bool initDatabase();
    bool checkUserExists();

    bool addUser(const QString &email, const QString &password);
    bool addData(const QString &username, const QString &password, const QString &website, const QString &url);

    bool verifyLogin(const QString &password);

private:
    QSqlDatabase db ;

};

#endif