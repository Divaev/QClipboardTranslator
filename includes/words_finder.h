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
    void setCurrentWord(const QString&);

signals:
    void sendResultOut(const QMap<QString, QString> &);
    void sendErrorNotification(const QString&);
    void wordIsReceived(const QString&);
    void setDictPath(const QString&);
    void stopWordsFinderThread();

};


class WordsFinder: public QObject {

    Q_OBJECT

    QString current_word;

    pugi::xml_document current_dictionary;

    static struct ErrorMessages {
        QString wordIsNotFound = "The word is not found!";
    } errorMessages;

public:

    QMap<QString, QString> translateTheSingleWorld(const pugi::xml_node&) const;

    ~WordsFinder();

public slots:
    void initTheDict(const QString& dict_path);
    void findTheSingleWord(const QString&);


signals:
    void translationIsReady(const QMap<QString, QString>&);
    void wordIsNotFound(const QString& = errorMessages.wordIsNotFound);
};



class WordsFinderThread: public QThread {
    Q_OBJECT
    QString dict_path;
    WordsFinder* wordsFinder;
    WordsReceiver* wordsReceiver;
    ~WordsFinderThread();
public:
    WordsFinderThread(WordsReceiver* receiver, const QString& dict_path,
                      QObject* parent = nullptr);
    void run() override;
    WordsFinder* getWordsFinder();
};

#endif // WORDS_FINDER_H
