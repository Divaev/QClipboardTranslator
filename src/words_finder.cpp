#include "words_finder.h"
#include <cstring>

WordsFinder::WordsFinder(): current_word(new char[MAX_WORD_SIZE]) {

}

pugi::xml_node WordsFinder::findTheSingleWorld(const char* word) const {
    pugi::xml_node main_node = current_dictionary.child("xdxf");
    pugi::xml_node found_node = current_dictionary.find_node([word](const pugi::xml_node& curr_node) {
                                                                    return strcmp(curr_node.value(), word) ? false: true;
                                                                });

    return found_node;
}

//const char* WordsFinder::translateTheSingleWorld(const pugi::xml_node& found_node) const {
QMap<QString, QString> WordsFinder::translateTheSingleWorld(const pugi::xml_node& found_node) const {
    std::cout << "Source word: " << found_node.value() << std::endl;
    auto node = found_node.parent();
    node = node.next_sibling();

    QMap<QString, QString> translationResult;

    while(node != nullptr) {

        if(node.type() == pugi::xml_node_type::node_element) {
            std::cout << "Transcription: " << node.child_value() << std::endl;
            //translationResult["translation"] = node.child_value();
        }
        else if(node.type() == pugi::xml_node_type::node_pcdata) {
            std::cout << "Translation: " << node.value() << std::endl;
            translationResult["transcription"] = node.value();
        }
        node = node.next_sibling();
    }

    //std::cout << "Translation: " << node.value() << std::endl;
    //return QMap<QString, QString>({QPair<QString, QString>("tr", resTranscription),
                                   //QPair<QString, QString> ("res", resTranslation)});
    return translationResult;
}


bool WordsFinder::initTheDict(const char* dict_path) {
    pugi::xml_parse_result result = current_dictionary.load_file(dict_path);

    return result;
}

WordsFinder::FinderError::FinderError(const char* err): errorMsg(err) {

}

void WordsFinder::FinderError::typeError() const {
    std::cout << errorMsg << std::endl;
}

const char *WordsFinder::FinderError::getError() const {
    return errorMsg;
}

WordsFinder::~WordsFinder() {

}

