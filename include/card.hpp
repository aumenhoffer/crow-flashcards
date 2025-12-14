#ifndef CARD_HPP
#define CARD_HPP

#include <string>

class Card {
    private:
        std::string term;
        std::string definition;
    public:
        Card(const std::string term, const std::string definition);
        std::string getTerm();
        std::string getDefinition();
        std::string setTerm(const std::string term);
        std::string setDefinition(const std::string definition);

};

#endif