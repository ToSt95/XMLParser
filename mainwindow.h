#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtXml>

namespace Ui {
class MainWindow;
}

class QDomDocument;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void saveToTxt();
    void saveToXml();
    void addRecord();
    void loadTXT();
    void loadXML();

private:
    Ui::MainWindow *ui;
    QStringList m_editedLines;
    QList<QStringList> m_splited;
    QString m_path;
    QDomDocument m_xmlBOM;
    QStringList* m_headers;

    void loadDataToView();
};

#endif // MAINWINDOW_H
