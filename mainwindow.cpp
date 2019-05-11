#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QStringListModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{



    ui->setupUi(this);
    handleFileLoad(m_path,m_lines);
    splitBySeparator(m_lines);
    ui->tableWidget->setColumnCount(m_splited.first().count());
    ui->tableWidget->setRowCount(m_splited.count());


    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::add);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::save);

    for ( int row = 0; row < m_splited.size(); ++row ) {
        for ( int column = 0; column < m_splited[row].size(); ++column ) {
            ui->tableWidget->setItem(row, column, new QTableWidgetItem(m_splited[row][column]));
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleFileLoad(const QString &path, QStringList lines)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "Can not open file";
        return;
    }

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        m_lines.append(line);
    }
}

void MainWindow::splitBySeparator(QStringList list)
{
    for(auto line : m_lines) {
        m_splited.append(line.split(";"));
        qDebug() << m_splited;
    }
}

void MainWindow::save()
{
    qDebug() << "SAVE";
    QFile file(m_path);
    if (file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream data( &file );
        QStringList strList;

        for( int r = 0; r < ui->tableWidget->rowCount(); ++r )
        {
            strList.clear();
            for( int c = 0; c < ui->tableWidget->columnCount(); ++c )
            {
                QTableWidgetItem* item = ui->tableWidget->item(r,c);
                if (!item || item->text().isEmpty())
                {
                    ui->tableWidget->setItem(r,c,new QTableWidgetItem(" "));
                }
                strList << ui->tableWidget->item( r, c )->text();

            }
           data << strList.join(";");
        }
        statusBar()->showMessage("Saved", 3000);
        file.close();
    }


}

void MainWindow::add()
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
}

