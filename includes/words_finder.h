#ifndef WORDS_FINDER_H
#define WORDS_FINDER_H
#include "pugixml.hpp"
#include <iostream>
#include <QMap>
#include <QPair>
#include <QObject>
#include <QThread>
#include <QDebug>

#define MAX_WORD_SIZE 128


class WordsReceiver: public QObject {
    Q_OBJECT
    QString input_word;
public:
    WordsReceiver(QObject* parent = nullptr);

//public slots:

    //void receiveFromClipboard(const QString&);
    void setCurrentWord(const QString&);

signals:
    void sendResultOut(const QMap<QString, QString> &);

    void wordIsReceived(const QString&);
    //void sendWordOut(const QMap<QString, QString> &);
};


class WordsFinder: public QObject {

    Q_OBJECT

    QString current_word;

    pugi::xml_document current_dictionary;


public:

    class FinderError;

    //pugi::xml_node findTheSingleWorld(const QString&) const;

    //const char* translateTheSingleWorld(const pugi::xml_node&) const;

    QMap<QString, QString> translateTheSingleWorld(const pugi::xml_node&) const;
    void initTheDict(const QString& dict_path);

    ~WordsFinder();

public slots:
    void findTheSingleWord(const QString&);


signals:
    void translationIsReady(const QMap<QString, QString>&);
};

class WordsFinder::FinderError {
    const char* errorMsg;
public:
    FinderError(const char* err);
    void typeError() const;
    const char* getError() const;
};


class WordsFinderThread: public QThread {
    Q_OBJECT
    QString dict_path;
    WordsFinder* wordsFinder;
    WordsReceiver* wordsReceiver;
public:
    WordsFinderThread(WordsReceiver* receiver, const QString& dict_path,
                      QObject* parent = nullptr);
    void run() override;
    WordsFinder* getWordsFinder();
};

#endif // WORDS_FINDER_H
