/****************************************************************************
** Authors: Dario Menasce, Stefano Terzo
**
** I.N.F.N. Milan-Bicocca
** Piazza  della Scienza 3, Edificio U2
** Milano, 20126
**
****************************************************************************/

#include <iostream>

#include <QBrush>
#include <QtGui>

#include <TBrowser.h>

#include "mainwindow.h"
#include "mdichild.h"
#include "mdisubwindow.h"

//===========================================================================
MainWindow::MainWindow()
{
    path_ = QString(getenv("MonicelliDir")) ;
    this->initialize() ;

    mdiArea      = new QMdiArea;
    windowMapper = new QSignalMapper(this);

    mdiArea->setGeometry(10,10,mainWindowW_,mainWindowH_);
    this   ->setGeometry(10,10,mainWindowW_,mainWindowH_);

    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(  Qt::ScrollBarAsNeeded);

    QLinearGradient gradient(0,mainWindowW_,0,mainWindowH_);
    gradient.setColorAt(0, QColor::fromRgbF(10, 10, 10, 1));
    gradient.setColorAt(1, QColor::fromRgbF(19, 19, 19, 0));

    QBrush mdiAreaBrush(gradient);
    mdiArea->setBackground(mdiAreaBrush);

    setCentralWidget(mdiArea);

    connect(mdiArea,
            SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this,
            SLOT  (updateMenus())                    );
    connect(windowMapper,
            SIGNAL(mapped(QWidget*)),
            this,
            SLOT  (setActiveSubWindow(QWidget*))     );
    connect(this,
            SIGNAL(destroyed()),
            this,
            SLOT  (cleanClose())                     ) ;

    createActions  ();
    createMenus    ();
    createToolBars ();
    createStatusBar();
    updateMenus    ();

    readSettings   ();

    setWindowTitle(tr("Monicelli: the track reconstruction program of the Fermilab test-beam"));

    setUnifiedTitleAndToolBarOnMac(true);

    theTabWidget_  = NULL ;
    theHNavigator_ = NULL ;
    theXMLEditor_  = NULL ;

    theFileEater_ = new fileEater();
    theHManager_  = new HManager(theFileEater_);
    theFileEater_->setHManger(theHManager_);
}

//===========================================================================
void MainWindow::initialize()
{
    mainWindowW_ = 1200 ;
    mainWindowH_ =  800 ;
    mainPanelW_  = 1010 ;
    mainPanelH_  =  780 ;
    xmlEditorW_  =  970 ;
    xmlEditorH_  =  735 ;
}

//===========================================================================
void MainWindow::cleanClose()
{
    STDLINE("Closing everything",ACRed) ;
    exit(0) ;
}


//===========================================================================
void MainWindow::closeEvent(QCloseEvent *event)
{
    STDLINE("Closing everything",ACRed) ;

    mdiArea->closeAllSubWindows();
    if (mdiArea->currentSubWindow())
    {
        event->ignore();
    }
    else
    {
        writeSettings();
        event->accept();
    }

    qApp->exit() ;
}

//===========================================================================
void MainWindow::newFile()
{
    MdiChild *child = createMdiChild();
    child->newFile();
    child->show();
}

//===========================================================================
void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        QMdiSubWindow *existing = findMdiChild(fileName);
        if (existing)
        {
            mdiArea->setActiveSubWindow(existing);
            return;
        }

        MdiChild *child = createMdiChild();
        if (child->loadFile(fileName))
        {
            statusBar()->showMessage(tr("File loaded"), 2000);
            child->show();
        }
        else
        {
            child->close();
        }
    }
}

//===========================================================================
void MainWindow::save()
{
    if (activeMdiChild() && activeMdiChild()->save())
        statusBar()->showMessage(tr("File saved"), 2000);
}

//===========================================================================
void MainWindow::saveAs()
{
    if (activeMdiChild() && activeMdiChild()->saveAs())
        statusBar()->showMessage(tr("File saved"), 2000);
}

//===========================================================================
void MainWindow::cut()
{
    if (activeMdiChild())
        activeMdiChild()->cut();
}

//===========================================================================
void MainWindow::copy()
{
    if (activeMdiChild())
        activeMdiChild()->copy();
}

//===========================================================================
void MainWindow::paste()
{
    if (activeMdiChild())
        activeMdiChild()->paste();
}

//===========================================================================
void MainWindow::about()
{
    QMessageBox msgBox;

    msgBox.setText("To get online help for <b>Monicelli</b> click Open");
    msgBox.setInformativeText("This requires Firefox to be installed on the local machine");
    msgBox.setStandardButtons(QMessageBox::Open | QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    if( msgBox.exec() == QMessageBox::Open )
    {
        system("firefox http://hal9000.mib.infn.it/~menasce/Monicelli &") ;
    }
}

//===========================================================================
void MainWindow::updateMenus()
{
    //    bool hasMdiChild = (activeMdiChild() != 0);
    //    saveAct     ->setEnabled(hasMdiChild);
    //    saveAsAct   ->setEnabled(hasMdiChild);
    //    pasteAct    ->setEnabled(hasMdiChild);
    //    closeAct    ->setEnabled(hasMdiChild);
    //    closeAllAct ->setEnabled(hasMdiChild);
    //    tileAct     ->setEnabled(hasMdiChild);
    //    cascadeAct  ->setEnabled(hasMdiChild);
    //    nextAct     ->setEnabled(hasMdiChild);
    //    previousAct ->setEnabled(hasMdiChild);
    //    separatorAct->setVisible(hasMdiChild);

    //    bool hasSelection = (activeMdiChild() &&
    //                         activeMdiChild()->textCursor().hasSelection());
    //    cutAct ->setEnabled(hasSelection);
    //    copyAct->setEnabled(hasSelection);
}

//===========================================================================
void MainWindow::updateWindowMenu()
{
    windowMenu->clear       ();
    //    windowMenu->addAction   (closeAct);
    //    windowMenu->addAction   (closeAllAct);
    //    windowMenu->addSeparator();
    //    windowMenu->addAction   (tileAct);
    //    windowMenu->addAction   (cascadeAct);
    //    windowMenu->addSeparator();
    //    windowMenu->addAction   (nextAct);
    //    windowMenu->addAction   (previousAct);
    //    windowMenu->addAction   (separatorAct);

    QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
    separatorAct->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i)
    {
        MdiChild *child = qobject_cast<MdiChild *>(windows.at(i)->widget());

        if( child )
        {
            QString text;
            if (i < 9)
            {
                text = tr("&%1 %2").arg(i + 1)
                       .arg(child->userFriendlyCurrentFile());
            }
            else
            {
                text = tr("%1 %2").arg(i + 1)
                       .arg(child->userFriendlyCurrentFile());
            }

            QAction *action  = windowMenu->addAction(text);
            action->setCheckable(true);
            connect(action,
                    SIGNAL(triggered()),
                    windowMapper,
                    SLOT  (map()));
            windowMapper->setMapping(action, windows.at(i));
            connect(action,
                    SIGNAL(triggered()),
                    windowMapper,
                    SLOT  (map()));
            action ->setChecked(child == activeMdiChild());
        }
    }
}

//===========================================================================
MdiChild *MainWindow::createMdiChild()
{
    MdiChild *child = new MdiChild;
    mdiArea->addSubWindow(child);

    connect(child,
            SIGNAL(copyAvailable(bool)),
            cutAct,
            SLOT  (setEnabled(bool))  );
    connect(child,
            SIGNAL(copyAvailable(bool)),
            copyAct,
            SLOT  (setEnabled(bool))  );

    return child;
}

//===========================================================================
void MainWindow::createActions()
{
    QString iconPath ;

    //    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    //    newAct->setShortcuts(QKeySequence::New);
    //    newAct->setStatusTip(tr("Create a new file"));
    //    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    //    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    //    openAct->setShortcuts(QKeySequence::Open);
    //    openAct->setStatusTip(tr("Open an existing file"));
    //    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    //    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    //    saveAct->setShortcuts(QKeySequence::Save);
    //    saveAct->setStatusTip(tr("Save the document to disk"));
    //    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    //    saveAsAct = new QAction(tr("Save &As..."), this);
    //    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    //    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    //    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    //    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
    //    cutAct->setShortcuts(QKeySequence::Cut);
    //    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
    //                            "clipboard"));
    //    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    //    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    //    copyAct->setShortcuts(QKeySequence::Copy);
    //    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
    //                             "clipboard"));
    //    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    //    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    //    pasteAct->setShortcuts(QKeySequence::Paste);
    //    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
    //                              "selection"));
    //    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    //    closeAct = new QAction(tr("Cl&ose"), this);
    //    closeAct->setStatusTip(tr("Close the active window"));
    //    connect(closeAct, SIGNAL(triggered()),
    //            mdiArea, SLOT(closeActiveSubWindow()));

    //    closeAllAct = new QAction(tr("Close &All"), this);
    //    closeAllAct->setStatusTip(tr("Close all the windows"));
    //    connect(closeAllAct, SIGNAL(triggered()),
    //            mdiArea, SLOT(closeAllSubWindows()));

    //    tileAct = new QAction(tr("&Tile"), this);
    //    tileAct->setStatusTip(tr("Tile the windows"));
    //    connect(tileAct, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

    //    cascadeAct = new QAction(tr("&Cascade"), this);
    //    cascadeAct->setStatusTip(tr("Cascade the windows"));
    //    connect(cascadeAct, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

    //    nextAct = new QAction(tr("Ne&xt"), this);
    //    nextAct->setShortcuts(QKeySequence::NextChild);
    //    nextAct->setStatusTip(tr("Move the focus to the next window"));
    //    connect(nextAct, SIGNAL(triggered()),
    //            mdiArea, SLOT(activateNextSubWindow()));

    //    previousAct = new QAction(tr("Pre&vious"), this);
    //    previousAct->setShortcuts(QKeySequence::PreviousChild);
    //    previousAct->setStatusTip(tr("Move the focus to the previous "
    //                                 "window"));
    //    connect(previousAct, SIGNAL(triggered()),
    //            mdiArea, SLOT(activatePreviousSubWindow()));

    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);

    aboutAct = new QAction(tr("&About Monicelli"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    iconPath  = path_ ;
    iconPath += "/images/newCanvas.png" ;
    hNavigatorAct = new QAction(QIcon(iconPath), tr("Histogram Navigator"), this);
    hNavigatorAct->setStatusTip(tr("Create a Histogram Navigator")) ;
    connect(hNavigatorAct,   SIGNAL(triggered()), this, SLOT(buildHNavigator()));

    iconPath  = path_ ;
    iconPath += "/images/editXML.png" ;
    editXMLAct = new QAction(QIcon(iconPath), tr("XML Geometry-Configuration file editor"), this);
    editXMLAct->setStatusTip(tr("Edit an xml geometry-configuration file")) ;
    connect(editXMLAct,      SIGNAL(triggered()), this, SLOT(editXMLPanel()));

    iconPath  = path_ ;
    iconPath += "/images/mainTabs.png" ;
    newMainPanelAct = new QAction(QIcon(iconPath), tr("Main Panel"), this);
    newMainPanelAct->setStatusTip(tr("Create a Main Panel")) ;
    connect(newMainPanelAct, SIGNAL(triggered()), this, SLOT(buildMainPanel()));
}

//===========================================================================
void MainWindow::enableMainPanelButton()
{
    newMainPanelAct->setEnabled(true) ;
    theTabWidget_ = NULL ;
}


//===========================================================================
void MainWindow::enableEditXMLButton()
{
    editXMLAct->setEnabled(true) ;
    theXMLEditor_ = NULL ;
}

//===========================================================================
void MainWindow::enableHNavigatorButton()
{
    hNavigatorAct->setEnabled(true) ;
    theHNavigator_ = NULL ;
}

//===========================================================================
void MainWindow::editXMLPanel()
{
    if( !theXMLEditor_ )
    {
        theXMLEditor_ = new XMLEditor(this) ;

        if( !theXMLEditor_->isActive() )
        {
            theXMLEditor_ = NULL ;
            return ;
        }

        mdiSubWindow * cSw = (mdiSubWindow*)mdiArea->addSubWindow(theXMLEditor_) ;

        editXMLAct->setEnabled(false) ;

        connect(cSw,
                SIGNAL(destroyed()           ),
                this,
                SLOT  (enableEditXMLButton()));
        cSw->setGeometry(5,
                         5,
                         theXMLEditor_->geometry().width(),
                         theXMLEditor_->geometry().height()) ;

        cSw->show() ;
    }

}

//===========================================================================
void MainWindow::editXMLPanel(QString fileName)
{
    if( !theXMLEditor_ )
    {
        theXMLEditor_ = new XMLEditor(fileName) ;

        if( !theXMLEditor_->isActive() )
        {
            theXMLEditor_ = NULL ;
            return ;
        }

        mdiSubWindow * cSw = (mdiSubWindow*)mdiArea->addSubWindow(theXMLEditor_) ;

        editXMLAct->setEnabled(false) ;

        connect(cSw,
                SIGNAL(destroyed()           ),
                this,
                SLOT  (enableEditXMLButton()));
        cSw->setGeometry(5,
                         5,
                         theXMLEditor_->geometry().width(),
                         theXMLEditor_->geometry().height()) ;

        cSw->show() ;
    }

}

//===========================================================================
void MainWindow::buildMainPanel()
{
    if( !theTabWidget_ )
    {
        theTabWidget_ = new mainTabs(this) ;

        mdiSubWindow * cSw = (mdiSubWindow*)mdiArea->addSubWindow(theTabWidget_) ;

        newMainPanelAct->setEnabled(false) ;

        connect(cSw,
                SIGNAL(destroyed()             ),
                this,
                SLOT  (enableMainPanelButton()));
        cSw->setGeometry(5,5,mainPanelW_,mainPanelH_) ;

        cSw->show() ;
    }

    theTabWidget_->collectExistingWidgets(this);

}

//===========================================================================
void MainWindow::buildHNavigator()
{
    if( !theHNavigator_)
    {
        theHNavigator_ = new HNavigator(this) ;

        cSw = (mdiSubWindow*)mdiArea->addSubWindow(theHNavigator_) ;

        hNavigatorAct->setEnabled(false) ;

        connect(cSw, SIGNAL(destroyed()), this, SLOT(enableHNavigatorButton()));

        cSw->setGeometry(1015,5,theHNavigator_->width()+8,theHNavigator_->height()+40) ;

        cSw->show() ;
    }

    theHNavigator_->collectExistingWidgets(this);

}

//===========================================================================
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    //    fileMenu->addAction(newAct);
    //    fileMenu->addAction(openAct);
    //    fileMenu->addAction(saveAct);
    //    fileMenu->addAction(saveAsAct);
    //    fileMenu->addSeparator();
    QAction *action = fileMenu->addAction(tr("Switch layout direction"));
    connect(action, SIGNAL(triggered()), this, SLOT(switchLayoutDirection()));
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    //    editMenu->addAction(cutAct);
    //    editMenu->addAction(copyAct);
    //    editMenu->addAction(pasteAct);

    windowMenu = menuBar()->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

//===========================================================================
void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    //    fileToolBar->addAction(newAct);
    //    fileToolBar->addAction(openAct);
    //    fileToolBar->addAction(saveAct);

    fileToolBar->addAction(newMainPanelAct) ;
    fileToolBar->addAction(hNavigatorAct) ;
    fileToolBar->addAction(editXMLAct) ;

    editToolBar = addToolBar(tr("Edit"));
    //    editToolBar->addAction(cutAct);
    //    editToolBar->addAction(copyAct);
    //    editToolBar->addAction(pasteAct);
}

//===========================================================================
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

//===========================================================================
void MainWindow::readSettings()
{
    QSettings settings("CMS", "Monicelli");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    this->move(pos);
    this->resize(size);
}

//===========================================================================
void MainWindow::writeSettings()
{
    QSettings settings("CMS", "Monicelli");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

//===========================================================================
MdiChild *MainWindow::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
        return qobject_cast<MdiChild *>(activeSubWindow->widget());
    return 0;
}

//===========================================================================
QMdiSubWindow *MainWindow::findMdiChild(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
        MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
        if (mdiChild->currentFile() == canonicalFilePath)
            return window;
    }
    return 0;
}

//===========================================================================
void MainWindow::switchLayoutDirection()
{
    if (layoutDirection() == Qt::LeftToRight)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
}

//===========================================================================
void MainWindow::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;
    mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}
