#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "QList"
#include "QListIterator"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#define MAX_SIZE 31


template <typename keyType, typename valueType>
class HashTable
{

    struct Node{
        keyType key;
        valueType value;
        bool operator==(const Node &another){return (key == another.key);}
        QJsonObject toJson(){
            return QJsonObject({
                                   qMakePair(QString("key"), QJsonValue(
                                        this->key)),
                                   qMakePair(QString("value"), QJsonValue(
                                        this->value))
                               });
        }
    };

private:
    QList<Node> list[MAX_SIZE];
    valueType totalValue;
public:
    HashTable();
    HashTable(const HashTable &hashTable);
    unsigned int getIndexByHash(keyType &key) const;
    void clear();
    unsigned int numItems() const;
    bool containtsKey(keyType &key) const;
    void readFromFile(QString& fileName);
    void writeToFile(QString& fileName) const;
    void add(keyType &key, valueType &value);
    void remove(keyType &key);
    valueType sum() const;
    bool operator==(const HashTable &hashTable) const;
    valueType& operator[](keyType &key) const;
    QList<keyType> getKeys() const;
    ~HashTable();
};

template <typename T, typename U>
HashTable<T,U>::HashTable()
{
    this->totalValue = 0;
}

template <typename T, typename U>
HashTable<T,U>::HashTable(const HashTable &hashTable){
    for (int i = 0; i < MAX_SIZE; ++i){
        list[i] = hashTable.list[i];
    }
    this->totalValue = hashTable.totalValue;
}

//template <typename T, typename U>
//unsigned int HashTable<T, U>::getIndexByHash(T &key){
//    unsigned int h = 0;
//    foreach(auto p, key){
//        h = h * 31 + static_cast<unsigned int>(p);

//    }
//    return h % MAX_SIZE;
//}

//template<>
//unsigned int HashTable<QString, int>::getIndexByHash(QString &key){
//    unsigned int h = 0;
//    foreach(auto p, key){
//        unsigned int t = 0;
//        t = p.unicode();
//        h = h * 31 + t;

//    }
//    return h % MAX_SIZE;
//}

template<typename T, typename U>
unsigned int HashTable<T, U>::getIndexByHash(T &key) const{
    unsigned int h = 0;
    foreach(auto p, key){
        unsigned int t = 0;
        t = p.unicode();
        h = h * 31 + t;

    }
    return h % MAX_SIZE;
}


template <typename T, typename U>
void HashTable<T,U>::clear(){
    for(int i = 0; i < MAX_SIZE; ++i){
        list[i].clear();
    }
    this->totalValue = 0;
}

template <typename T, typename U>
void HashTable<T,U>::add(T &key, U& value){
    unsigned int index = this->getIndexByHash(key);
    this->totalValue += value;
    Node *cur = new Node;
    if(cur){
        cur->key = key;
        cur->value = value;
        if(list[index].contains(*cur)){
            list[index][list[index].indexOf(*cur)] = *cur;
            return;
        }
        list[index].append(*cur);
    }
}

template <typename T, typename U>
unsigned int HashTable<T,U>::numItems() const{
    unsigned int number = 0;
    for (int i = 0; i < MAX_SIZE; ++i){
        number += list[i].length();
    }
    return number;
}


template <typename T, typename U>
void HashTable<T,U>::remove(T &key){
    Node *temp = new Node;
    temp->key = key;
    QList<Node> cur = list[this->getIndexByHash(key)];

    if (cur.indexOf(*temp) != -1) this
        ->totalValue -= cur[cur.indexOf(*temp)].value;

    list[this->getIndexByHash(key)].removeAll(*temp);
}

template <typename T, typename U>
bool HashTable<T,U>::containtsKey(T &key) const{
    for (int i = 0; i < MAX_SIZE; ++i){
        Node *temp = new Node;
        temp->key = key;
        if(list[i].contains(*temp)) return true;
    }
    return false;
}


template <typename T, typename U>
U HashTable<T,U>::sum() const{
    return totalValue;
}

template <typename T, typename U>
void HashTable<T,U>::writeToFile(QString& fileName) const{
    QFile file(fileName+".json");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return;

    QJsonObject data;
    int t = 0;

    for(int i = 0; i < MAX_SIZE; ++i){
        QList<Node> temp = list[i];
        for(int j = 0; j < temp.length(); ++j){
            Node cur = temp[j];
            data.insert(QString::number(t), cur.toJson());
            ++t;
        }
    }

    file.write(QJsonDocument(data).toJson(QJsonDocument::Indented));
    file.close();
}

//template <>
//void HashTable<std::string,int>::readFromFile(QString &fileName){
//    this->clear();
//    QFile file(fileName+".json");
//    if (!file.open(QIODevice::ReadOnly)) return;
//    QJsonObject temp = QJsonDocument::fromJson(file.readAll()).object();
//    for(const auto& i: temp.keys()){
//        QJsonObject object = temp.value(i).toObject();
//        std::string key = object["key"].toString().toStdString();
//        int value = object["value"].toInt();
//        this->add(key, value);
//    }
//    file.close();
//}

template<typename T, typename U>
void HashTable<T,U>::readFromFile(QString &fileName){
    this->clear();

    QFile file(fileName+".json");
    if (!file.open(QIODevice::ReadOnly)) return;
    QJsonObject temp = QJsonDocument::fromJson(file.readAll()).object();

    for(const auto& i: temp.keys()){
        QJsonObject object = temp.value(i).toObject();
        QString key = object["key"].toString();
        int value = object["value"].toInt();
        this->add(key, value);
        this->totalValue += value;
    }

    file.close();

}

template <typename T, typename U>
bool HashTable<T,U>::operator==(const HashTable &hashTable) const{
    for(int i = 0; i < MAX_SIZE; ++i){
        if (list[i].length() != hashTable.list[i].length()) return false;
        foreach(Node cur, list[i]) {
            if (!(hashTable.list[i].contains(cur)
                  && hashTable.list[i][hashTable.list[i]
                  .indexOf(cur)].value == cur.value)) return false;
        }
    }
    return true;
}

template <typename T, typename U>
U& HashTable<T,U>::operator[](T &key) const{
    unsigned int index = this->getIndexByHash(key);
    Node *temp = new Node;
    temp->key = key;
    QList<Node> cur = list[index];
    if (cur.indexOf(*temp) != -1) return cur[cur.indexOf(*temp)].value;
    else throw -1;
}

template <typename T, typename U>
QList<T> HashTable<T,U>::getKeys() const{
    QList<T> data;
    for (int i = 0; i < MAX_SIZE; ++i){
        foreach(Node cur, list[i]){
            data.append(cur.key);
        }
    }
    return data;
}

template <typename T, typename U>
HashTable<T,U>::~HashTable(){
    this->clear();
}



#endif // HASHTABLE_H
