#include "card.hpp"

Card::Card(const std::string term, const std::string definition) : term(term), definition(definition) {};
std::string Card::getTerm() { return term; };
std::string Card::getDefinition() { return definition; }

std::string Card::setTerm(const std::string term) {
    this->term = term;
    return term;
};
std::string Card::setDefinition(const std::string definition) {
    this->definition = definition;
    return definition;
};
    