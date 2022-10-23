#include "includes/maintranslatorwin.h"

#include <QApplication>
#include <QtConcurrent/QtConcurrent>

#include "clipboardchkr.h"
#include "words_finder.h"

#include <cstring>
#include <iostream>

int main(int argc, char *argv[])
{
    //const char *old_style_string = new char[10];

    /*
    char *const old_style_string = new char[10]; //This is constant pointer
    const char *old_style_string_1 = "Hello world!";
    std::strcpy(old_style_string, "hello");
    std::cout << old_style_string << std::endl;
    std::cout << old_style_string_1 << std::endl;
    */


    //ClipboardChecker clipChkr;
    //clipChkr.testXML();

    /*
    WordsFinder test_translator;
    try {
        test_translator.initTheDict("dict.xdxf");
    }
    catch(const WordsFinder::FinderError &err) {
        err.typeError();
    }
    */

    //pugi::xml_node found_node = test_translator.findTheSingleWorld("apple");
    //test_translator.translateTheSingleWorld(found_node);

    //clipChkr.testClipboard();


    qRegisterMetaType<QMap<QString, QString>>();            //it's necessary, because we need to pass
                                                            //QMap<QString, QString> with signal

    QApplication a(argc, argv);

    /*
    while(true) {
        qInfo() << QApplication::clipboard()->text();
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
    */

    /*
    QtConcurrent::run([=] {
        while(true) {
            qInfo() << QApplication::clipboard()->text();
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }

        });
    */


    MainTranslatorWin w;
    //w.getClipboardThread()->setPriority(QThread::HighPriority);
    //QApplication::clipboard()->moveToThread(w.getClipboardThread());
    w.getWordsFinderThread()->start();

    w.show();
    return a.exec();
}
