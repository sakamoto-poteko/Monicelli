#include <iostream>
#include <sstream>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QString>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startPB_clicked()
{
    char * path = getenv("MonicelliDir") ;
    if( path == NULL )
    {
        std::cout << "Sorry: MonicelliDir is undefined" << std::endl ;
        exit(0) ;
    }
    \
    std::string MonicelliDir = path ;

    QString startingInputPath = "./" ;
    QString inputDir = QFileDialog::getExistingDirectory ( this , "Input directory", startingInputPath) ;
    if (inputDir.isEmpty())  return ;

    QString startingOutputPath = "./" ;
    QString outputDir = QFileDialog::getExistingDirectory ( this , "Output directory", startingOutputPath) ;
    if (outputDir.isEmpty())  return ;

    QString filePath = inputDir ;
    QStringList fileNames = QFileDialog::getOpenFileNames(this,"Input files",filePath,"Monicelli reconstructed data (*.root)");

    if( fileNames.size() == 0 ) return ;

    std::cout << "You selected the following input  directory: " <<  inputDir.toStdString() << std::endl ;
    std::cout << "You selected the following output directory: " << outputDir.toStdString() << std::endl ;

    std::stringstream ss ;
    ss << MonicelliDir << "/convert.pl " << inputDir.toStdString() << " " << outputDir.toStdString() << " " ;

    for(int i=0; i<fileNames.size(); ++i)
    {
        std::string fName = fileNames.at(i).toStdString() ;
        int last = fName.find_last_of("/") ;
        fName.replace(0,last+1,"") ;
        last = fName.find(".root") ;
        fName.replace(last,fName.size()+1,"") ;
        ss << fName << " " ;
    }

    std::cout << "Command to be executed: " << ss.str() << std::endl ;

    system(ss.str().c_str()) ;
}
