#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "hashtable.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_addButton_clicked();

    void on_deleteButton_clicked();

    void on_keyToAdd_textChanged(const QString &arg1);

    void on_valueToAdd_textChanged(const QString &arg1);

    void on_keyToDelete_textChanged(const QString &arg1);

    void on_findButton_clicked();

    void on_keyToFind_textChanged(const QString &arg1);

    void on_clearTable_clicked();

    void on_fileNameEdit_textChanged(const QString &arg1);

    void on_saveButton_clicked() const;

    void on_loadButton_clicked();

private:

    void fillListWidget();

    HashTable<QString, int> hashTable;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
