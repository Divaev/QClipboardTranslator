#include "clipboardchkr.h"
#include <iostream>
#include <cstring>
#include "pugixml.hpp"

int ClipboardChecker::testXML() const {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("/home/ingenigger/Projects/QClipboardTranslator/dict.xdxf");
    if (!result) {
        std::cout << "Can not open .xml" << std::endl;
        return -1;
    }
    else {
        std::cout << "It's OK" << std::endl;
        std::cout << "Info: " << result.description() << std::endl;
    }

    for (pugi::xml_node tool: doc.child("Profile").child("Tools").children("Tool")) {
        int timeout = tool.attribute("Timeout").as_int();

        if (timeout > 0)
            std::cout << "Tool " << tool.attribute("Filename").value() << " has timeout " << timeout << "\n";
     }

    for (pugi::xml_node word: doc.child("Hello")) {
        std::cout << "word is found" << std::endl;
    }
    for (pugi::xml_node word: doc.child("to")) {
        std::cout << "word is found" << std::endl;
    }



    pugi::xml_node word = doc.child("xdxf");
    pugi::xml_node found_node = word.find_node([](const pugi::xml_node &curr_node){if (!strcmp(curr_node.value(), "final reminder"))
                                                        return true;
                                                       else
                                                        return false;
                                                        });
    std::cout << "Found_node value: " << found_node.value() << std::endl;
    std::cout << "Found_node name: " << found_node.parent().name() << std::endl;


    std::cout << "doc.child_value(): " << doc.child_value() << std::endl;
    std::cout << "doc.first_child(): " << doc.first_child().first_child().value() << std::endl;

    for (pugi::xml_node word: doc.child("note")) {
        std::cout << "word is found" << std::endl;
        //std::cout << word.attribute("to").value() << std::endl;
        //std::cout << word.name() << std::endl;
        auto curr_node = word.first_child();
        if(curr_node == nullptr) {
            std::cout << "null node was detected!" << std::endl;
        }
        std::cout << "word.value(): " << word.value() << std::endl;
        std::cout << "First child value: " << word.first_child().value() << std::endl;
        std::cout << "Last child value: " << word.last_child().first_child().value() << std::endl;
        std::cout << "Next_sibling: " << word.next_sibling().first_child().value() << std::endl;
        std::cout << "word.child_value(): " << word.child_value() << std::endl;

    }

//    std::cout << "first child of doc: " << doc.first_child().value() << std::endl;


    for (pugi::xml_node word: doc.child("name")) {
        std::cout << "name word is found" << std::endl;
    }
    for (pugi::xml_node word: doc.child("Reminder")) {
        std::cout << "Reminder word is found" << std::endl;
    }
    for (pugi::xml_node word: doc.child("name")) {
        std::cout << "name word is found" << std::endl;
    }

    std::cout << "End" << std::endl;


}
