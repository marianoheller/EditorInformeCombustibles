QT      +=  webkitwidgets network widgets printsupport

CONFIG += c++11

HEADERS =   previewer.h \
            mainwindow.h
SOURCES =   main.cpp \
            previewer.cpp \
            mainwindow.cpp
FORMS   =   previewer.ui

RESOURCES += \
    templates.qrc

