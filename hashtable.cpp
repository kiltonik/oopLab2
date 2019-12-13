#include "hashtable.h"


template<>
unsigned int HashTable<QString, int>::getIndexByHash(QString &key) const{
    unsigned int h = 0;
    foreach(auto p, key){
        unsigned int t = 0;
        t = p.unicode();
        h = h * 31 + t;

    }
    return h % MAX_SIZE;
}
