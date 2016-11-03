#ifndef USERTYPES
#define USERTYPES

#include <QString>

struct purchase {
    int goodId = 0;
    int goodCode = 0;
    int goodCount = 0;
    double sum = 0;
    double goodPrice = 0;
    double goodPricePDV = 0;
    QString goodName;
    QString goodProvider;
};

struct user {
    int iduser, postId;
    QString login;
};

#endif // USERTYPES

