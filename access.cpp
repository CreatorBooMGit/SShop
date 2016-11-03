#include "access.h"
#include <QVariant>
#include <QDebug>

Access::Access()
{

}

Access::Access(QString log, QSqlQuery *q)
{
    setQuery(q);
    setLogin(log);
}

Access::~Access()
{

}

void Access::setLogin(QString log)
{
    login = log;

    query->prepare("SELECT positions.idposition "
                   "FROM positions, workers "
                   "WHERE workers.idposition = positions.idposition AND workers.login = :login");
    query->bindValue(":login", login);
    query->exec();
    query->next();
    idpost = query->value(0).toInt();
}

void Access::setQuery(QSqlQuery *q)
{
    query = q;
}

bool Access::checkAccess(QString access)
{
    query->prepare("SELECT access_post.idaccess "
                   "FROM access_post, access "
                   "WHERE access_post.access = access.idaccess AND access.name = :name AND access_post.post = :post");
    query->bindValue(":name", access);
    query->bindValue(":post", idpost);
    query->exec();
    if(query->size() <= 0) return false;
    else
        return true;
}

