#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QMap>
#include <QRegExp>
#include "regex"
#include <string>
#include <iostream>
#include <QDebug>
#include <cmath>
#include <qDebug>


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
int MainWindow::operandsFind(QString str)
{
    for (int i = 0; i < operands.length(); i++)
    {
        if (operands[i].first == str)
            return i;
    }
    return -1;
}
int MainWindow::operatorsFind(QString str)
{
    for (int i = 0; i < operators.length(); i++)
    {
        if (operators[i].first == str)
            return i;
    }
    return -1;
}


void MainWindow::operatorsSearch(std::string code)
{
    std::string codeCopy = code;
    std::smatch match;

    //operators
    // add to comments all operators as remind

    std::regex opers(R"((>>>)|(<<=)|(>>=)|(>=)|(>>)|(!=)|(\|=)|(\^=)|(&=)|([^\/<\+\-\*\\\%\=]([\+\-\*\/\%\.\=\<\>])[^\/=><])|(while) *[\(]|(for) *[\(]|(println) *[\(]|(print) *[\(]|(object +(.+) )|(if) *[\(]|(\{)|(\()|(&&)|(\|\|)|(!|\^|\|&|~)|(<<<)|(<<)|([\+\-\*\/\%\=\<\>\&\|\^\~]=))");

    while(std::regex_search(codeCopy, match, opers))
    {
        QString str = QString::fromStdString(match[1].str() + match[2].str() + match[3].str() + match[4].str() + match[5].str() + match[6].str() + match[7].str() + match[8].str() + match[9].str() + match[11].str() + match[12].str() + match[14].str() + match[15].str() + match[17].str() + match[19].str() + match[20].str() + match[21].str() + match[22].str() + match[23].str() + match[24].str() + match[25].str() + match[26].str() + match[27].str() + match[28].str());
        int index = operatorsFind(str);
        if (index >= 0)
        {
            operators[index].second++;
        }
        else
        {
            std::pair<QString, int> curr;
            curr.first = str;
            curr.second = 1;
            operators.append(curr);
        }
        codeCopy = match.suffix();
    }

    std::regex breaket(R"((def.+|List *)(\())");
    codeCopy = code;
    while(std::regex_search(codeCopy, match, breaket))
    {
        QString str = "(";
        int index = operatorsFind(str);
        if (index >= 0)
        {
            operators[index].second--;
        }
        codeCopy = match.suffix();
    }


    for(int j = 0; j < unicOperators.length(); j++)
    {
        std::regex customOperators("(" + unicOperators[j].toStdString() + ")\\(");
        codeCopy = code;
        while(std::regex_search(codeCopy, match, customOperators))
        {
            int index = operatorsFind(QString::fromStdString(match[1].str()));
            if (index >= 0)
            {
                operators[index].second++;
                int bracket = operatorsFind("(");
                if(bracket != -1)
                {
                    operators[bracket].second--;
                }

            }
            else
            {
                std::pair<QString, int> curr;
                curr.first = QString::fromStdString(match[1].str());
                curr.second = 1;
                operators.append(curr);
            }
            codeCopy = match.suffix();
        }
    }
}


void MainWindow::unicOperatorsSearch(std::string code)
{
    std::string codeCopy = code;
    std::smatch match;

    std::regex operatorsReg(R"(def +([a-zA-Z_][a-zA-Z0-9_]*) *\()");

    while(std::regex_search(codeCopy, match, operatorsReg))
    {
        unicOperators.push_back(QString::fromStdString(match[1].str()));
        codeCopy = match.suffix();
    }
}


void MainWindow::constantsFind(std::string code)
{
    std::string codeCopy = code;

    std::regex numberReg(R"((-?[0-9]*[,.]?[0-9]+))");
    std::smatch match;

    int i = 0;
    while(std::regex_search(codeCopy, match, numberReg))
    {
        i = operandsFind(QString::fromStdString(match[1].str()));
        if(i == -1)
        {
            std::pair<QString,int> curr {QString::fromStdString(match[1].str()),1};
            operands.append(curr);

        }else
        {
            operands[i].second++;
        }
        codeCopy = match.suffix();
    }

}


void MainWindow::operandsSearch(std::string code)
{
    std::string codeCopy = code;

    std::regex varlsReg(R"((va[l|r] +([a-zA-Z_][a-zA-Z0-9_]*)))");
    std::smatch match;

    while(std::regex_search(codeCopy, match, varlsReg))
    {
        unicOperands.push_back(QString::fromStdString(match[2].str()));
        codeCopy = match.suffix();
    }

    int i = 0;
    for(int j = 0; j < unicOperands.length();j++)
    {
        std::regex varlsReg("[ \({.:$\"](" + unicOperands[j].toStdString() + ")[ \)}.:\"]");
        codeCopy = code;
        while(std::regex_search(codeCopy, match, varlsReg))
        {
            i = operandsFind(QString::fromStdString(match[1].str()));
            if(i == -1)
            {
                std::pair<QString,int> curr {QString::fromStdString(match[1].str()),1};
                operands.push_back(curr);
            }else
            {
                operands[i].second += 1;
            }
            codeCopy = match.suffix();
        }
    }

}


void MainWindow::on_testBtn_clicked()
{
    codeProcessing();
}

void MainWindow::on_downlCodeBtn_clicked()
{
    originalCode = std::string();
    comments = std::string();
    unicOperands = QVector<QString>();
    unicOperators = QVector<QString>();
    operands = QVector<std::pair<QString, int>>();
    operators = QVector<std::pair<QString, int>>();


    QString filename = QFileDialog::getOpenFileName();
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    originalCode = file.readAll().toStdString();
}

void MainWindow::codeProcessing()
{
    std::regex commentRegex(R"((/\*([^*]|[\r\n]|(\*+([^*/]|[\r\n])))*\*+/)|((//.*)))");

    originalCode = std::regex_replace(originalCode, commentRegex, "");

    stringProcessing(originalCode);

    ui->testLbl->setText(QString::fromStdString(originalCode));

    operandsSearch(originalCode);
    constantsFind(originalCode);

    unicOperatorsSearch(originalCode);
    operatorsSearch(originalCode);


    {
        int flag = 0;
        ui->tableOperands->setRowCount(operands.length());
        for (int i = 0; i < operands.length(); i++)
        {

            ui->tableOperands->setItem(i, 0, new QTableWidgetItem(operands[i].first));
            ui->tableOperands->setItem(i, 1, new QTableWidgetItem(QString::number(operands[i].second)));
        }

        flag = 0;
        ui->tableOperators->setRowCount(operators.length());
        for (int i = 0; i < operators.length(); i++)
        {

            ui->tableOperators->setItem(i, 0, new QTableWidgetItem(operators[i].first));
            ui->tableOperators->setItem(i, 1, new QTableWidgetItem(QString::number(operators[i].second)));
        }

    }
    //base metrics


    int unic_operands = 0;
    int unic_operators = 0;
    int all_operands = 0;
    int all_operators = 0;

    for (int i = 0; i < operands.length(); i++)
    {
        if (operands[i].second != 0)
        {
            unic_operands++;
        }
    }

    for (int i = 0; i < operators.length(); i++)
    {
        if (operators[i].second != 0)
        {
            unic_operators++;
        }
    }

    for (int i = 0; i < operands.length(); i++)
    {
        all_operands += operands[i].second;
    }

    for (int i = 0; i < operators.length(); i++)
    {
        all_operators += operators[i].second;
    }

    ui->unic_operands->setNum(unic_operands);
    ui->unic_operators->setNum(unic_operators);
    ui->all_operands->setNum(all_operands);
    ui->all_operators->setNum(all_operators);

    //extra metrics


    ui->dict->setNum(unic_operands + unic_operators);
    ui->len->setNum(all_operands + all_operators);
    ui->vol->setNum(int((all_operands + all_operators) * log2(unic_operands + unic_operators)));
}

void MainWindow::stringProcessing(std::string code)
{
    std::string codeCopy = code;
    std::regex strReg(R"((s)?["'](.*)["'*])");
    std::regex varInStrReg(R"(\$\{([^\}]*)|\$([a-zA-Z_][a-zA-Z0-9_]*))");

    std::smatch match, match1;
    int i = 0;
    originalCode += '\n';
    while(std::regex_search(codeCopy, match, strReg))
    {
        if(match[1].str() == "s")
        {
            std::string tempStr = match[2].str();
            while(std::regex_search(tempStr,match1,varInStrReg))
            {
                originalCode += match1[1].str() + match1[2].str() + '\n';
                tempStr = match1.suffix();
            }
        }

        i = operandsFind(QString::fromStdString(match[2].str()));
        if(i == -1)
        {
            std::pair<QString,int> curr {QString::fromStdString(match[2].str()),1};
            operands.push_back(curr);
        }else
        {
            operands[i].second++;
        }
        codeCopy = match.suffix();
    }
    originalCode = std::regex_replace(originalCode, strReg, "");
}

