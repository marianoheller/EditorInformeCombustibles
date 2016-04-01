/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QSet>
#include "previewer.h"

const QString rsrcTemplatesPath = ":/templates";

#define TABLA_HEAD_1    3
#define TABLA_HEAD_2    4
#define TABLA_HEAD_3    5
#define TABLA_HEAD_4    6
#define TABLA_HEAD_5    7
#define TABLA_DATCOL_1  10
#define TABLA_DATCOL_2  11
#define TABLA_DATCOL_3  12
#define TABLA_DATCOL_4  13
#define TABLA_DATCOL_5  14


#define FILENAME_TEMPLATE00       ":/templates/00_template_init.htm"
#define FILENAME_TEMPLATE01       ":/templates/01_template_head.htm"
#define FILENAME_TEMPLATE02       ":/templates/02_template_bodyInit.htm"
#define FILENAME_TEMPLATE03       ":/templates/03_template_tableInit.htm"
#define FILENAME_TEMPLATE04_1     ":/templates/04_template_tableHead.htm"
#define FILENAME_TEMPLATE04_2     ":/templates/04_template_tableRow.htm"
#define FILENAME_TEMPLATE04_3     ":/templates/04_template_tableSpacer.htm"
#define FILENAME_TEMPLATE05_1     ":/templates/05_template_closeUpTable.htm"
#define FILENAME_TEMPLATE05_2     ":/templates/05_template_closeUpTotal.htm"


enum {
    eDataType_GasOil=0,
    eDataType_Nafta_g2,
    eDataType_Nafta_g3,
    eDataType_TOTAL
};


QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE


//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void open();
    void CSVOpen();
    void openUrl();
    void save();
    void about();
    void updateTextEdit();
    void filePrintPdf();
//! [0]

//! [1]
private:
    Previewer *centralWidget;
    QMenu *fileMenu;
    QMenu *helpMenu;
    QAction *openAct;
    QAction *openCSV;
    QAction *openUrlAct;
    QAction *saveAct;
    QAction *pdfAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *aboutQtAct;


    void createActions();
    void createMenus();
    void setStartupText();
    void setDataTypeSets();
    bool parseLoadCSV(const QString &f);
    QString transformCSVtoHTML(QVector<QStringList> * data);
    QVector< QSet<QString> > dataTypeGroups;
    QSet<QString> dataTypeExpetions;
};
//! [1]

#endif
