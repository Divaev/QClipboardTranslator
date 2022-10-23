

#ifndef MAINTRANSLATORWIN_H
#define MAINTRANSLATORWIN_H

#include <QMainWindow>
#include <QTextCursor>
#include <QClipboard>
#include "words_finder.h"
//#include "clipboardchkr.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainTranslatorWin; }
QT_END_NAMESPACE



class MainTranslatorWin : public QMainWindow
{
    Q_OBJECT


    //WordsFinder translator;
    QString current_word;

    //ClipboardChecker clipboardChecker;
    //ClipboardThread *clipboardThread;
    QClipboard *clipboard;
    WordsReceiver *wordsReceiver;
    WordsFinderThread* wordsFinderThread;


public:
    MainTranslatorWin(QWidget *parent = nullptr);
    //ClipboardThread* getClipboardThread();
    WordsFinderThread* getWordsFinderThread();
    ~MainTranslatorWin();

private:
    Ui::MainTranslatorWin *ui;
    //void translateInputWord(const pugi::xml_node& found_node);

public slots:

    //void findInputWord(QString = "");
    void printResultTranslation(const QMap<QString, QString>&);
    void makeTranslateAvailable();

signals:
    //void wordIsNotFound();
    void setDictPath(const QString&);
};
#endif // MAINTRANSLATORWIN_H
