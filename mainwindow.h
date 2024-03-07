#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_pushButton_clicked();



    void comments_Separation(std::string code);

    int operandsFind(QString str);

    int operatorsFind(QString str);

    void constantsFind(std::string code);

    void operandsShow();
    void operatorsShow();



    void operandsSearch(std::string code);

    void operatorsSearch(std::string code);

    void unicOperatorsSearch(std::string code);

    void commentsCorrection(std::string code);

    void on_testBtn_clicked();

    void on_downlCodeBtn_clicked();

private:
    Ui::MainWindow *ui;
    std::string originalCode = std::string();
    std::string comments = std::string();
    QVector<QString> unicOperands = QVector<QString>();
    QVector<QString> unicOperators = QVector<QString>();
    QVector<std::pair<QString, int>> operands = QVector<std::pair<QString, int>>();
    QVector<std::pair<QString, int>> operators = QVector<std::pair<QString, int>>();
};
#endif // MAINWINDOW_H
