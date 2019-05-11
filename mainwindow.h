#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void handleFileLoad(const QString& path, QStringList lines);
    void splitBySeparator(QStringList list);

public slots:
    void save();
    void add();

private:
    Ui::MainWindow *ui;
    QStringList m_lines;
    QStringList m_editedLines;
    QList<QStringList> m_splited;
    const QString m_path{"/home/piotr/Projects/Qt/lab2/data"};

};

#endif // MAINWINDOW_H
