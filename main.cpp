#include "mainwindow.h"

#include <QApplication>
#include "hashtable.h"
#include "iostream"
#include <cassert>

void assertConstructor(){
    HashTable<QString, int> table1;
    assert(table1.numItems() == 0);
}

void assertAdding(){
    HashTable<QString, int> table1;
    unsigned int a = table1.numItems();
    QString b = "buka";
    int x = 1;
    table1.add(b,x);
    unsigned int c = table1.numItems();
    assert(c == ++a);
    table1.add(b, x);
    unsigned int d = table1.numItems();
    assert(c == d);
}

void assertContainment(){
    HashTable<QString, int> table1;
    QString b = "buka";
    int x = 1;
    table1.add(b,x);
    assert(table1.containtsKey(b) == true);

}

void assertIndexing(){
    HashTable<QString, int> table1;
    QString b = "buka";
    int x = 1;
    table1.add(b,x);
    assert(table1[b] == x);
}

void assertCopyConstructor(){
    HashTable<QString, int> table1;
    QString b = "buka", c = "baka";
    int x = 1, y = 2;
    table1.add(b, x);
    table1.add(c, y);
    HashTable<QString, int> table2(table1);
    assert(table1 == table2);

}

void assertDeleting(){
    HashTable<QString, int> table1;
    QString b = "buka", c = "baka";
    int x = 1, y = 2;
    table1.add(b, x);
    table1.add(c, y);
    unsigned int num = table1.numItems();
    table1.remove(c);
    assert(--num == table1.numItems());
    table1.remove(c);
    assert(table1.numItems() == num);
}

void assertContainment2(){
    HashTable<QString, int> table1;
    QString b = "buka", c = "baka";
    int x = 1, y = 2;
    table1.add(b, x);
    table1.add(c, y);
    table1.remove(c);
    assert(!table1.containtsKey(c));
}

void assertClearing(){
    HashTable<QString, int> table1;
    QString b = "buka", c = "baka";
    int x = 1, y = 2;
    table1.add(b, x);
    table1.add(c, y);
    table1.clear();
    HashTable<QString, int> table2;
    assert(table1 == table2);
}

void assertCopying2(){
    HashTable<QString, int> table1;
    QString b = "buka", c = "baka";
    int x = 1, y = 2;
    table1.add(b, x);
    table1.add(c, y);
    HashTable<QString, int> table2(table1);
    assert(table1 == table2);
    table2.remove(c);
    assert(!(table1 == table2));
}

void assertFileWork(){
    HashTable<QString, int> table1;
    QString b = "buka", c = "baka";
    int x = 1, y = 2;
    table1.add(b, x);
    table1.add(c, y);
    QString fileName("1.json");
    table1.writeToFile(fileName);
    HashTable<QString, int> table2;
    table2.readFromFile(fileName);
    table2.writeToFile(fileName.replace("1","2"));
    assert(table1 == table2);
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    assertConstructor();
    assertAdding();
    assertCopyConstructor();
    assertContainment();
    assertIndexing();
    assertDeleting();
    assertContainment2();
    assertClearing();
    assertCopying2();
    assertFileWork();
    std::cout << "Everything is ok" << std::endl;
    MainWindow w;
    w.show();
    return a.exec();
}
