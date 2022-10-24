#include "includes/maintranslatorwin.h"

#include <QApplication>
#include <QtConcurrent/QtConcurrent>

#include "words_finder.h"

#include <cstring>
#include <iostream>

int main(int argc, char *argv[])
{

    qRegisterMetaType<QMap<QString, QString>>();            //it's necessary, because we need to pass
                                                            //QMap<QString, QString> with signal

    QApplication a(argc, argv);

    MainTranslatorWin w;

    w.getWordsFinderThread()->start();                    //start another thread
                                                          //for words seeking in the dictionary
    w.show();
    return a.exec();
}
