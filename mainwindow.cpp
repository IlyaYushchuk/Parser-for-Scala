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

void MainWindow::on_pushButton_clicked()
{
    std::string code = originalCode;
    std::smatch match;

    std::regex printcoms(R"(["'].+["'])");

    while(std::regex_search(code, match, printcoms))
    {
        std::string com = match[0].str();
        std::smatch match2;
        while(std::regex_search(com, match2, printcoms))
        {
            QString str = QString::fromStdString(match2[1].str() + match2[2].str() + match2[3].str() + match2[4].str() + match2[5].str() + match2[6].str() + match2[7].str() + match2[8].str() + match2[9].str() + match2[11].str() + match2[13].str() + match2[14].str() + match2[15].str() + match2[16].str() + match2[17].str() + match2[19].str() + match2[20].str() + match2[21].str() + match2[22].str() + match2[23].str() + match2[24].str() + match2[25].str() + match2[26].str() + match2[27].str() + match2[28].str());
            int index = operatorsFind(str);
            if (index >= 0)
            {
                operators[index].second--;
            }
            com = match2.suffix();
        }

        com = match[0].str();
        std::smatch match3;
        std::regex varincoms(R"(\$\{.+\})");
        while(std::regex_search(com, match3, varincoms))
        {
            std::string com2 = match3[0].str();
            std::smatch match4;
            while(std::regex_search(com2, match4, printcoms))
            {
                QString str = QString::fromStdString(match4[1].str() + match4[2].str() + match4[3].str() + match4[4].str() + match4[5].str() + match4[6].str() + match4[7].str() + match4[8].str() + match4[9].str() + match4[11].str() + match4[13].str() + match4[14].str() + match4[15].str() + match4[16].str() + match4[17].str() + match4[19].str() + match4[20].str() + match4[21].str() + match4[22].str() + match4[23].str() + match4[24].str() + match4[25].str() + match4[26].str() + match4[27].str() + match4[28].str());
                int index = operatorsFind(str);
                if (index >= 0)
                {
                    operators[index].second++;
                }
                com2 = match4.suffix();
            }
            com = match3.suffix();
        }
        code = match.suffix();
    }

    //file.seek(0);
    //code = file.readAll().toStdString();
    std::regex breaket(R"((while *|for *|if *|println *|print *|def.+|List *)(\())");

    while(std::regex_search(code, match, breaket))
    {
        QString str = QString::fromStdString(match[2].str());
        int index = operatorsFind(str);
        if (index >= 0)
        {
            operators[index].second--;
        }
        code = match.suffix();
    }

    {
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


        //table view


        int flag = 0;
        ui->tableOperands->setRowCount(unic_operands);
        for (int i = 0; i < operands.length(); i++)
        {
            if (operands[i].second == 0)
            {
                flag++;
                continue;
            }
            ui->tableOperands->setItem(i - flag, 0, new QTableWidgetItem(operands[i].first));
            ui->tableOperands->setItem(i - flag, 1, new QTableWidgetItem(QString::number(operands[i].second)));
        }

        flag = 0;
        ui->tableOperators->setRowCount(unic_operators);
        for (int i = 0; i < operators.length(); i++)
        {
            if (operators[i].second == 0)
            {
                flag++;
                continue;
            }
            ui->tableOperators->setItem(i - flag, 0, new QTableWidgetItem(operators[i].first));
            ui->tableOperators->setItem(i - flag, 1, new QTableWidgetItem(QString::number(operators[i].second)));
        }


        //extra metrics


        ui->dict->setNum(unic_operands + unic_operators);
        ui->len->setNum(all_operands + all_operators);
        ui->vol->setNum(int((all_operands + all_operators) * log2(unic_operands + unic_operators)));
    }
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

void MainWindow::commentsCorrection(std::string code)
{
    std::string codeCopy = code;
    std::smatch match;

    //operands correction

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
                //operands.push_back(curr);
            }else
            {
                operands[i].second--;
            }
            codeCopy = match.suffix();
        }
    }

    codeCopy = code;

    std::regex numberReg(R"((-?[0-9]*[,.]?[0-9]+))");


    while(std::regex_search(codeCopy, match, numberReg))
    {
        i = operandsFind(QString::fromStdString(match[1].str()));
        if(i == -1)
        {
            std::pair<QString,int> curr {QString::fromStdString(match[1].str()),1};
            // operands.append(curr);

        }else
        {
            operands[i].second--;
        }
        codeCopy = match.suffix();
    }


    codeCopy = code;
    std::regex stringReg(R"("(.*)\")");

    while(std::regex_search(codeCopy, match, stringReg))
    {
        i = operandsFind(QString::fromStdString(match[1].str()));
        if(i == -1)
        {
            std::pair<QString,int> curr {QString::fromStdString(match[1].str()),1};
            //operands.push_back(curr);
        }else
        {
            operands[i].second--;
        }
        codeCopy = match.suffix();
    }

    //operators correction
    std::regex opers(R"((>>>)|(<<=)|(>>=)|(>=)|(>>)|(!=)|(\|=)|(\^=)|(&=)|([^\/<\+\-\*\\\%\=]([\+\-\*\/\%\.\=\<\>])[^\/=><])|(while) *[\(]|(for) *[\(]|(println) *[\(]|(print) *[\(]|(object +(.+) )|(if) *[\(]|(\{)|(\()|(&&)|(\|\|)|(!|\^|\|&|~)|(<<<)|(<<)|([\+\-\*\/\%\=\<\>\&\|\^\~]=))");

    while(std::regex_search(codeCopy, match, opers))
    {
        QString str = QString::fromStdString(match[1].str() + match[2].str() + match[3].str() + match[4].str() + match[5].str() + match[6].str() + match[7].str() + match[8].str() + match[9].str() + match[11].str() + match[12].str() + match[14].str() + match[15].str() + match[17].str() + match[19].str() + match[20].str() + match[21].str() + match[22].str() + match[23].str() + match[24].str() + match[25].str() + match[26].str() + match[27].str() + match[28].str());
        int index = operatorsFind(str);
        if (index >= 0)
        {
            operators[index].second--;
        }
        else
        {
            std::pair<QString, int> curr;
            curr.first = str;
            curr.second = 1;
            //operators.append(curr);
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
            operators[index].second++;
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
                operators[index].second--;

            }
            else
            {
                std::pair<QString, int> curr;
                curr.first = QString::fromStdString(match[1].str());
                curr.second = 1;
                //operators.append(curr);
            }
            codeCopy = match.suffix();
        }
    }


    //вычитание из оператора . всех точек в вещественных литералахё

    codeCopy = originalCode;
    std::regex dotReg(R"(([0-9]\.[0-9]))");


    while(std::regex_search(codeCopy, match, dotReg))
    {
        int i = operatorsFind(".");
        operators[i].second--;
        codeCopy = match.suffix();
    }

}


void MainWindow::comments_Separation(std::string code)
{
    std::string codeCopy = code;
    std::regex oneLineCommentsReg(R"(\/\/(.*))");
    std::smatch match;
    while(std::regex_search(codeCopy, match, oneLineCommentsReg))
    {
        comments += match[1].str() +'\n';
        codeCopy = match.suffix();
    }


    codeCopy = code;
    std::regex multiLineCommentsReg(R"(\/\*([\s\S]*?)\*\/)");
    while(std::regex_search(codeCopy, match, multiLineCommentsReg))
    {
        comments += match[1].str() +'\n';
        codeCopy = match.suffix();
    }

    //    codeCopy = code;
    //    std::regex strConstReg(R"("(.*)\")");
    //    while(std::regex_search(codeCopy, match, strConstReg))
    //    {
    //        comments += match[1].str() +'\n';
    //        codeCopy = match.suffix();
    //    }

    //    codeCopy = code;
    //    std::regex numConstReg(R"((-?[0-9]*[,.]?[0-9]+))");
    //    while(std::regex_search(codeCopy, match, numConstReg))
    //    {
    //        comments += match[1].str() +'\n';
    //        codeCopy = match.suffix();
    //    }

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


    codeCopy = code;
    std::regex stringReg(R"("(.*)\")");

    while(std::regex_search(codeCopy, match, stringReg))
    {
        i = operandsFind(QString::fromStdString(match[1].str()));
        if(i == -1)
        {
            std::pair<QString,int> curr {QString::fromStdString(match[1].str()),1};
            operands.push_back(curr);
        }else
        {
            operands[i].second++;
        }
        codeCopy = match.suffix();
    }
}

void MainWindow::operandsShow()
{
    QString allOperands = "";
    for(int i = 0; i < operands.length();i++)
    {
        allOperands += operands[i].first + "\n";
    }
    allOperands += QString::number(operands.length());
    ui->testLbl->setText(allOperands);
}

void MainWindow::operatorsShow()
{
    QString allOperators = "";
    for(int i = 0; i < operators.length();i++)
    {
        allOperators += operators[i].first + " " + QString::number(operators[i].second) + "\n";
    }
    ui->testLbl->setText(allOperators);
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
    comments_Separation(originalCode);
    operandsSearch(originalCode);
    constantsFind(originalCode);

    unicOperatorsSearch(originalCode);
    operatorsSearch(originalCode);

    commentsCorrection(comments);


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

