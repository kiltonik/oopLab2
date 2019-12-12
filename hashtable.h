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
                                   qMakePair(QString("key"), QJsonValue(this->key)),
                                   qMakePair(QString("value"), QJsonValue(this->value))
                               });
        }
    };

private:
    QList<Node> list[MAX_SIZE];
public:
    HashTable();
    HashTable(const HashTable &hashTable);
    unsigned int getIndexByHash(keyType &key);
    void clear();
    unsigned int numItems();
    bool containtsKey(keyType &key);
    void readFromFile(QString& fileName);
    void writeToFile(QString& fileName);
    void add(keyType &key, valueType &value);
    void remove(keyType &key);
    int sum();
    bool operator==(const HashTable &hashTable);
    valueType& operator[](keyType &key);
    ~HashTable();
};

template <typename T, typename U>
HashTable<T,U>::HashTable()
{
    //for (int i = 0; i < MAX_SIZE; ++i) list[i] = QList<Node>();
}

template <typename T, typename U>
HashTable<T,U>::HashTable(const HashTable &hashTable){
    for (int i = 0; i < MAX_SIZE; ++i){
        list[i] = hashTable.list[i];
    }
}

//template <>
//unsigned int HashTable<std::string, int>::getIndexByHash(std::string &key){
//    unsigned int h = 0;
//    foreach(auto p, key){
//        h = h * 31 + static_cast<unsigned int>(p);

//    }
//    return h % MAX_SIZE;
//}

template<>
unsigned int HashTable<QString, int>::getIndexByHash(QString &key){
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
}

template <typename T, typename U>
void HashTable<T,U>::add(T &key, U& value){
    int index = this->getIndexByHash(key);
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
unsigned int HashTable<T,U>::numItems(){
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
    list[this->getIndexByHash(key)].removeAll(*temp);
}

template <typename T, typename U>
bool HashTable<T,U>::containtsKey(T &key){
    for (int i = 0; i < MAX_SIZE; ++i){
        Node *temp = new Node;
        temp->key = key;
        if(list[i].contains(*temp)) return true;
    }
    return false;
}


template <typename T, typename U>
int HashTable<T,U>::sum(){
    int sum = 0;
    QList<Node> *first = list;
    while(*first){
        foreach(Node cur, first) sum += cur.value;
        first++;
    }
    return sum;
}

template <typename T, typename U>
void HashTable<T,U>::writeToFile(QString& fileName){
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

template<>
void HashTable<QString,int>::readFromFile(QString &fileName){
    this->clear();
    QFile file(fileName+".json");
    if (!file.open(QIODevice::ReadOnly)) return;
    QJsonObject temp = QJsonDocument::fromJson(file.readAll()).object();
    for(const auto& i: temp.keys()){
        QJsonObject object = temp.value(i).toObject();
        QString key = object["key"].toString();
        int value = object["value"].toInt();
        this->add(key, value);
    }
    file.close();

}

template <typename T, typename U>
bool HashTable<T,U>::operator==(const HashTable &hashTable){
    for(int i = 0; i < MAX_SIZE; ++i){
        if (list[i].length() != hashTable.list[i].length()) return false;
        foreach(Node cur, list[i]) {
            if (!(hashTable.list[i].contains(cur) && hashTable.list[i][hashTable.list[i].indexOf(cur)].value == cur.value)) return false;
        }
    }
    return true;
}

template <typename T, typename U>
U& HashTable<T,U>::operator[](T &key){
    unsigned int index = this->getIndexByHash(key);
    Node *temp = new Node;
    temp->key = key;
    QList<Node> cur = list[index];
    if (cur.indexOf(*temp) != -1) return cur[cur.indexOf(*temp)].value;
    else throw "Invalid key";
}

template <typename T, typename U>
HashTable<T,U>::~HashTable(){
    this->clear();
}



#endif // HASHTABLE_H
