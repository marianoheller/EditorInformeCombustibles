#include <QtWidgets>
#include <QtWebKitWidgets>
#include <QtPrintSupport/QPrinter>
#include <QDialog>
#include <QTimer>
#include "mainwindow.h"

//! [0]
MainWindow::MainWindow()
{
    createActions();
    createMenus();
    centralWidget = new Previewer(this);
    setCentralWidget(centralWidget);

    this->setWindowTitle("Editor - Informe Combustibles");
    centralWidget->editorBox->setVisible(false);

    setDataTypeSets();

    connect(centralWidget->webView, SIGNAL(loadFinished(bool)),
        this, SLOT(updateTextEdit()));
    setStartupText();
}
//! [0]

void MainWindow::setDataTypeSets()
{
    QSet<QString> auxSet;

    auxSet << "Gas Oil (grado 2)" << "FAME en Gas Oil" << "Azufre en Gas Oil";
    dataTypeGroups.push_back(auxSet);
    auxSet.clear();

    auxSet << "Nafta Super (grado 2)" << "RON y MON por IR" << "Azufre en Nafta Super";
    dataTypeGroups.push_back(auxSet);
    auxSet.clear();

    auxSet << "Nafta Ultra (grado 3)" << "RON y MON por IR" << "Azufre en Nafta Ultra";
    dataTypeGroups.push_back(auxSet);
    auxSet.clear();

    dataTypeExpetions << "RON y MON por IR";
}

//! [1]
void MainWindow::createActions()
{
    openAct = new QAction(tr("&Abrir..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing HTML file"));
    openAct->setDisabled(true);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    openCSV = new QAction(tr("&Abrir CSV..."), this);
    connect(openCSV, SIGNAL(triggered()), this, SLOT(CSVOpen()));

    openUrlAct = new QAction(tr("&Abrir URL..."), this);
    openUrlAct->setShortcut(tr("Ctrl+U"));
    openUrlAct->setStatusTip(tr("Open a URL"));
    openUrlAct->setDisabled(true);
    connect(openUrlAct, SIGNAL(triggered()), this, SLOT(openUrl()));
//! [1]

    saveAct = new QAction(tr("&Guardar"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the HTML file to disk"));
    saveAct->setDisabled(true);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    exitAct = new QAction(tr("Salir"), this);
    exitAct->setStatusTip(tr("Exit the application"));
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&Acerca de.."), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("Acerca de &Qt"), this);
    aboutQtAct->setStatusTip(tr("Muestra informacion sobre la libreria Qt"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    pdfAct = new QAction(tr("&Exportar PDF..."), this);
    connect(pdfAct, SIGNAL(triggered()), this, SLOT(filePrintPdf()));
}

//! [2]
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Archivo"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(openCSV);
    fileMenu->addAction(openUrlAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(pdfAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Ayuda"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}
//! [2]


void MainWindow::filePrintPdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export PDF",
                                                    QString(), "*.pdf");
    if (!fileName.isEmpty())
    {
        QMessageBox *mbox = new QMessageBox;
        mbox->setText("Documendo exportado.");
        mbox->setWindowTitle(tr("Información"));
        mbox->setStandardButtons(0);
        mbox->show();
        QTimer::singleShot(2000, mbox, SLOT(hide()));

        if (QFileInfo(fileName).suffix().isEmpty())
            fileName.append(".pdf");
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        //QMessageBox::information( this, "Información" , tr("Documendo exportado.") );
        centralWidget->webView->print(&printer);
    }
}


//! [3]
void MainWindow::about()
{
    QMessageBox::about(this, tr("Acerca de.."),
        tr("El <b>Editor de informes</b> permite editar "
           "los informes exportados con formato CSV."));
}
//! [3]

//! [4]
void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        // read from file
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }

        QTextStream out(&file);
        QString output = out.readAll();

        // display contents
        centralWidget->plainTextEdit->setPlainText(output);
        centralWidget->setBaseUrl(QUrl::fromLocalFile(fileName));
    }
}
//! [4]


void MainWindow::CSVOpen()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                              QString(), tr("CSV Files (*.csv);;All Files (*)"));
    if (!fn.isEmpty())
        parseLoadCSV(fn);
    centralWidget->previewButton->click();
}

//! [5]
void MainWindow::openUrl()
{
    bool ok;
    QString url = QInputDialog::getText(this, tr("Enter a URL"),
                  tr("URL:"), QLineEdit::Normal, "http://", &ok);

    if (ok && !url.isEmpty()) {
        centralWidget->webView->setUrl(url);
    }
}
//! [5]

//! [6]
void MainWindow::save()
{
    QString content = centralWidget->plainTextEdit->toPlainText();
    QString fileName = QFileDialog::getSaveFileName(this);

    if (!fileName.isEmpty()) {
        // save to file
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }

        QTextStream in(&file);
        in << content;
    }
}
//! [6]

//! [7]
void MainWindow::updateTextEdit()
{
    QWebFrame *mainFrame = centralWidget->webView->page()->mainFrame();
    QString frameText = mainFrame->toHtml();
    centralWidget->plainTextEdit->setPlainText(frameText);
}
//! [7]

//! [8]
void MainWindow::setStartupText()
{
    QString string = "<html><body><h1>Editor de informes</h1>"
                     " <p>Abra un archivo CSV para generar la tabla</p>"
                     " </body></html>";
    centralWidget->webView->setHtml(string);
}
//! [8]



bool MainWindow::parseLoadCSV(const QString &f)
{
    if (!QFile::exists(f))
        return false;
    QFile file(f);
    if (!file.open(QFile::ReadOnly))
        return false;

    QVector<QStringList> csvRawData;
    QTextStream s1(&file);
    while (!s1.atEnd()) {
        QStringList auxRowData;
        QString line=s1.readLine(); // reads line from file
        QRegExp reg("\",\"|\"");
        auxRowData=line.split(reg);
        csvRawData.append(auxRowData);
    }

    for ( int i=0 ; i<csvRawData.size() ; i++)
    {
        for (int j=0 ; j<csvRawData.at(i).size() ; j++)
        {
            //Remove all \\r
            QString returnDeleterString = csvRawData.at(i).at(j);
            int k;
            while( (k=returnDeleterString.indexOf("\r")) != -1)
                returnDeleterString.remove(k,1);
            //Trim
            returnDeleterString = returnDeleterString.trimmed();
            //And add a spacing on empty string, for html porpouses
            if (returnDeleterString.isEmpty())
                returnDeleterString = "&zwnj;";
            csvRawData[i].replace(j,returnDeleterString);


        }
    }

    QString htmlData = transformCSVtoHTML(&csvRawData);

    QByteArray data;
    data.append(htmlData.toUtf8());


    QTextCodec *codec = Qt::codecForHtml(data);
    QString str = codec->toUnicode(data);

    centralWidget->plainTextEdit->setPlainText(str);
    centralWidget->setBaseUrl(QUrl::fromLocalFile(f));

    return true;
}



QString MainWindow::transformCSVtoHTML(QVector<QStringList> * data)
{
    QString htmlParsedText;

    ///Init
    QFile file(FILENAME_TEMPLATE00);
    if (!file.open(QFile::ReadOnly))
        return QString("Template not found");
    htmlParsedText.append(file.readAll());
    file.close();

    ///Head
    file.setFileName(FILENAME_TEMPLATE01);
    if (!file.open(QFile::ReadOnly))
        return QString("Template not found");
    htmlParsedText.append(file.readAll());
    file.close();

    ///Body init
    file.setFileName(FILENAME_TEMPLATE02);
    if (!file.open(QFile::ReadOnly))
        return QString("Template not found");
    htmlParsedText.append(file.readAll());
    file.close();

    ///Table init
    file.setFileName(FILENAME_TEMPLATE03);
    if (!file.open(QFile::ReadOnly))
        return QString("Template not found");
    htmlParsedText.append(file.readAll());
    file.close();

    ///Table Data
    QString headCol_01_Ant = "";
    int dataType_last=0;
    bool fFistDataType = true;  //Para safar del primer spacer (antes del primer item)
    for ( int i=0 ; i<data->size() ; i++)
    {
        if ( data->at(i).at(TABLA_HEAD_1) != headCol_01_Ant)        //if different, insert header AND data
        {
            // Identify set
            int dataType_index=-1;
            for ( int dataType_aux=0 ; dataType_aux < dataTypeGroups.size() ; dataType_aux++)
            {
                if( dataTypeGroups.at(dataType_aux).contains( data->at(i).at(TABLA_HEAD_1) ) )
                {
                    //if ( data->at(i).at(TABLA_HEAD_1) == "RON y MON por IR" )   //Excepcion xq este campo se repite
                    if ( dataTypeExpetions.contains( data->at(i).at(TABLA_HEAD_1) ) )   //Excepcion xq este campo se repite
                        dataType_index = dataType_last;
                    else
                        dataType_index = dataType_aux;
                }
            }

            //Put Spacer
            if ( dataType_index != dataType_last && fFistDataType==false )
            {
                file.setFileName(FILENAME_TEMPLATE04_3);
                if (!file.open(QFile::ReadOnly))
                    return QString("Template not found");
                QString auxTemplateA = QString::fromLocal8Bit(file.readAll());
                htmlParsedText.append(auxTemplateA);
                file.close();
            }
            dataType_last = dataType_index;
            //Add Head
            file.setFileName(FILENAME_TEMPLATE04_1);
            if (!file.open(QFile::ReadOnly))
                return QString("Template not found");
            QString auxTemplate = QString::fromLocal8Bit(file.readAll());
            auxTemplate.replace("<HEAD_1>",data->at(i).at(TABLA_HEAD_1));
            auxTemplate.replace("<HEAD_2>",data->at(i).at(TABLA_HEAD_2));
            auxTemplate.replace("<HEAD_3>",data->at(i).at(TABLA_HEAD_3));
            auxTemplate.replace("<HEAD_4>",data->at(i).at(TABLA_HEAD_4));
            auxTemplate.replace("<HEAD_5>",data->at(i).at(TABLA_HEAD_5));
            htmlParsedText.append(auxTemplate);
            file.close();

            //Add Data
            file.setFileName(FILENAME_TEMPLATE04_2);
            if (!file.open(QFile::ReadOnly))
                return QString("Template not found");
            auxTemplate = QString::fromLocal8Bit(file.readAll());
            auxTemplate.replace("<DATO_1>",data->at(i).at(TABLA_DATCOL_1));
            auxTemplate.replace("<DATO_2>",data->at(i).at(TABLA_DATCOL_2));
            auxTemplate.replace("<DATO_3>",data->at(i).at(TABLA_DATCOL_3));
            auxTemplate.replace("<DATO_4>",data->at(i).at(TABLA_DATCOL_4));
            auxTemplate.replace("<DATO_5>",data->at(i).at(TABLA_DATCOL_5));
            htmlParsedText.append(auxTemplate);
            file.close();
        }
        else //else, continue with the same header
        {
            file.setFileName(FILENAME_TEMPLATE04_2);
            if (!file.open(QFile::ReadOnly))
                return QString("Template not found");
            QString auxTemplate = QString::fromLocal8Bit(file.readAll());
            auxTemplate.replace("<DATO_1>",data->at(i).at(TABLA_DATCOL_1));
            auxTemplate.replace("<DATO_2>",data->at(i).at(TABLA_DATCOL_2));
            auxTemplate.replace("<DATO_3>",data->at(i).at(TABLA_DATCOL_3));
            auxTemplate.replace("<DATO_4>",data->at(i).at(TABLA_DATCOL_4));
            auxTemplate.replace("<DATO_5>",data->at(i).at(TABLA_DATCOL_5));
            htmlParsedText.append(auxTemplate);
            file.close();
        }
        headCol_01_Ant = data->at(i).at(TABLA_HEAD_1);
        fFistDataType = false;
    }

    ///Close up
    file.setFileName(FILENAME_TEMPLATE05_1);
    if (!file.open(QFile::ReadOnly))
        return QString("Template not found");
    htmlParsedText.append(file.readAll());
    file.close();

    file.setFileName(FILENAME_TEMPLATE05_2);
    if (!file.open(QFile::ReadOnly))
        return QString("Template not found");
    htmlParsedText.append(file.readAll());
    file.close();

    return htmlParsedText;
}
