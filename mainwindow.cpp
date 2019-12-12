#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fillListWidget(){
    QList<QString> keys = this->hashTable.getKeys();
    foreach (QString key, keys) {
        ui->listWidget->addItem(QString("Name: ") + key
                                + QString("\nPrice: ")
                                + QString::number(this
                                                  ->hashTable[key]));
    }
    ui->totalValue->setText(QString::number(this->hashTable.sum()));
    if (this->hashTable.numItems() != 0) ui->clearTable->setEnabled(true);
    else ui->clearTable->setDisabled(true);
}

void MainWindow::on_addButton_clicked()
{
    QString key = ui->keyToAdd->text();
    int price;

    try {
        price = stoi(ui->valueToAdd->text().toStdString());
        ui->valueToAdd->clear();
    } catch (std::invalid_argument) {

        ui->valueToAdd->clear();

        QMessageBox::warning(this,
                             "Error",
                             "Value must contain only numbers"
                             );
        return;
    }

    ui->keyToAdd->clear();
    ui->valueToAdd->clear();

    this->hashTable.add(key, price);
    ui->listWidget->clear();
    this->fillListWidget();
    ui->addButton->setDisabled(true);
}

void MainWindow::on_deleteButton_clicked()
{
    QString key = ui->keyToDelete->text();
    ui->keyToDelete->clear();

    this->hashTable.remove(key);
    ui->listWidget->clear();
    this->fillListWidget();

    ui->deleteButton->setDisabled(true);
}

void MainWindow::on_findButton_clicked()
{
    QString key = ui->keyToFind->text();
    ui->keyToFind->clear();

    try{
        int temp = this->hashTable[key];
        ui->foundedKey->setText(key);
        ui->foundedValue->setText(QString::number(temp));
    }
    catch (int){
        QMessageBox::warning(this, "Error", "No such key");
    }

    ui->findButton->setDisabled(true);
}

void MainWindow::on_clearTable_clicked()
{
    ui->listWidget->clear();
    this->hashTable.clear();
    ui->clearTable->setDisabled(true);
}

void MainWindow::on_saveButton_clicked()
{
    QString fileName = ui->fileNameEdit->text();
    ui->fileNameEdit->clear();

    this->hashTable.writeToFile(fileName);
}

void MainWindow::on_loadButton_clicked()
{
    QString fileName = ui->fileNameEdit->text();
    ui->fileNameEdit->clear();
    ui->listWidget->clear();
    this->hashTable.readFromFile(fileName);
    this->fillListWidget();
}

void MainWindow::on_keyToAdd_textChanged(const QString &text)
{
    if (!text.isEmpty() && !ui->valueToAdd->text().isEmpty()) ui->addButton
            ->setEnabled(true);
    else ui->addButton->setDisabled(true);
}

void MainWindow::on_valueToAdd_textChanged(const QString &text)
{
    if (!text.isEmpty() && !ui->keyToAdd->text().isEmpty()) ui->addButton
            ->setEnabled(true);
    else ui->addButton->setDisabled(true);
}

void MainWindow::on_keyToDelete_textChanged(const QString &text)
{
    if (!text.isEmpty()) ui->deleteButton->setEnabled(true);
    else ui->deleteButton->setDisabled(true);
}

void MainWindow::on_keyToFind_textChanged(const QString &text)
{
    if (!text.isEmpty()) ui->findButton->setEnabled(true);
    else ui->findButton->setDisabled(true);
}

void MainWindow::on_fileNameEdit_textChanged(const QString &text)
{
    if(!text.isEmpty()){
        ui->saveButton->setEnabled(true);
        ui->loadButton->setEnabled(true);
    }
    else{
        ui->saveButton->setDisabled(true);
        ui->loadButton->setDisabled(true);
    }
}
