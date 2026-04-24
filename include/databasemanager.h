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

    bool addUser(const QString &email, const QString &passwordHash);

private:
    QSqlDatabase db ;

};

#endif