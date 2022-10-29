

#ifndef MAINTRANSLATORWIN_H
#define MAINTRANSLATORWIN_H

#include <QMainWindow>
#include <QTextCursor>
#include <QClipboard>
#include "words_finder.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainTranslatorWin; }
QT_END_NAMESPACE



class MainTranslatorWin : public QMainWindow
{
    Q_OBJECT

    QString current_word;

    QClipboard *clipboard;
    WordsReceiver *wordsReceiver;               //stays in GUI thread, receives translations
    WordsFinderThread* wordsFinderThread;       //finding in the dictionary goes to another thread

    void closeEvent(QCloseEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
public:
    MainTranslatorWin(QWidget *parent = nullptr);
    WordsFinderThread* getWordsFinderThread();
    ~MainTranslatorWin();

private:
    Ui::MainTranslatorWin *ui;

public slots:

    void printResultTranslation(const QMap<QString, QString>&);
    void printError(const QString&);
    void makeTranslateAvailable();

signals:
    //void setDictPath(const QString&);
    void stopWordsFinderThread();
};
#endif // MAINTRANSLATORWIN_H
