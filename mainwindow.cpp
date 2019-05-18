#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QStringListModel>
#include <QFileDialog>
#include <QFileInfo>
#include <QDomDocument>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::addRecord);
    connect(ui->saveToTxtButton, &QPushButton::clicked, this, &MainWindow::saveToTxt);
    connect(ui->saveToXMLButton, &QPushButton::clicked, this, &MainWindow::saveToXml);

    connect(ui->loadButton, &QPushButton::clicked, [this]() {
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::ExistingFile);
        dialog.setNameFilter(tr("Pliki (*.txt *.xml)"));
        dialog.exec();
        if(dialog.selectedFiles().isEmpty()) return;
        m_path = dialog.selectedFiles().first();

        QFileInfo info(m_path);
        // clear curent view
        ui->tableWidget->clear();
        m_splited.clear();

        m_headers = new QStringList;

        if(info.suffix() == "xml") {
            loadXML();
        } else {
            loadTXT();
        }

        if(m_splited.isEmpty()) return;
        for(int i=0; i<m_splited.first().size(); i++) {
            QString headerText = i<m_headers->size() ? m_headers->at(i) : "undefined";
            ui->tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(headerText));
        }

    });


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveToTxt()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save..."), "",
            tr("All Files (*)"));
    if(fileName.isEmpty()) return;

    QFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream data( &file );
        QStringList strList;

        for( int row = 0; row < ui->tableWidget->rowCount(); ++row )
        {
            strList.clear();
            for( int column = 0; column < ui->tableWidget->columnCount(); ++column )
            {
                QTableWidgetItem* item = ui->tableWidget->item(row,column);
                if (!item || item->text().isEmpty())
                {
                    ui->tableWidget->setItem(row,column,new QTableWidgetItem("-"));
                }
                strList << ui->tableWidget->item(row, column)->text();

            }
            data << strList.join(";");
            data << "\n";
        }
        statusBar()->showMessage("Saved", 3000);
        file.close();
    }
}

void MainWindow::saveToXml()
{
    QDomDocument document;
    QDomElement root = document.createElement("laptops");
    document.appendChild(root);

    for( int row = 0; row < ui->tableWidget->rowCount(); row++ )
    {
        QDomElement firstChild = document.createElement("laptop");
        root.appendChild(firstChild);

        for( int column = 0; column < ui->tableWidget->columnCount(); column++ )
        {
            QString headerName = ui->tableWidget->horizontalHeaderItem(column)->text();
            QDomElement manufacturer = document.createElement(headerName);
            firstChild.appendChild(manufacturer);

            QTableWidgetItem* item = ui->tableWidget->item(row,column);
            if (!item || item->text().isEmpty())
            {
                ui->tableWidget->setItem(row,column,new QTableWidgetItem("-"));
            }
            manufacturer.appendChild(document.createTextNode(ui->tableWidget->item(row, column)->text()));
        }
    }

    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save..."), "",
            tr("All Files (*)"));
    if(fileName.isEmpty()) return;

    QFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream stream(&file);
        stream << document.toString();
        file.close();
    }
}

void MainWindow::addRecord()
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
}

void MainWindow::loadTXT()
{
    // set headers
    m_headers->clear();
    m_headers->append({"manufacturer", "size", "reslution", "type", "touchscreen", "name", "physical_cores", "clock_speed"
                               ,"ram","storage", "type", "name", "memory", "os", "disc_reader"});
    QFile file(m_path);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Can not open file";
        return;
    }
    qDebug() << "File opened";

    QTextStream stream(&file);
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        m_splited.append(line.split(";"));
    };
    file.close();

    loadDataToView();
}

void MainWindow::loadXML()
{
    m_headers->append({"manufacturer", "size", "type", "touchscreen", "name", "physical_cores", "clock_speed"
                               ,"ram","storage", "type", "name", "memory", "os", "disc_reader"});

    QFile file(m_path);
    if (file.open(QIODevice::ReadOnly ))
    {
        m_xmlBOM.setContent(&file);
        file.close();
    }

    // root
    QDomElement root = m_xmlBOM.documentElement();
    QString rootName = root.tagName();
    // first child
    QDomElement Component = root.firstChild().toElement();

    // Loop while there is a child
    while(!Component.isNull())
    {
        QString firstChildName = Component.tagName();
        QDomElement Child=Component.firstChild().toElement();

        QStringList row;
        // Read each child of the component node
        while (!Child.isNull())
        {
            if(Child.childNodes().size() == 1) {
                row.append(Child.firstChild().toText().data());
            }

            if(Child.hasChildNodes()) {

                auto element = Child.firstChild().toElement();
                while (!element.isNull())
                {
                    row.append(element.firstChild().toText().data());
                    element = element.nextSibling().toElement();
                }
            }
            Child = Child.nextSibling().toElement();
        }
        m_splited.append(row);
        Component = Component.nextSibling().toElement();
    }

    loadDataToView();
}

void MainWindow::loadDataToView()
{
    if(m_splited.count()) {
        ui->tableWidget->setColumnCount(m_splited.first().count());
        ui->tableWidget->setRowCount(m_splited.count());

        for ( int row = 0; row < m_splited.count(); ++row ) {
            for ( int column = 0; column < m_splited[row].count(); ++column ) {
                ui->tableWidget->setItem(row, column, new QTableWidgetItem(m_splited[row][column]));
            }
        }
    } else {
        qDebug() << "File is empty";
    }
}
