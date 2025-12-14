#include "set.hpp"
#include <chrono>
#include <cstdint>
#include <stdexcept>

Set::Set(const std::string name) : name(name) {
    this->id = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
};

std::string Set::getName() { return name; }
uint Set::getId() { return id; }
std::string Set::getAuthor() { return author; }
std::string Set::getDescription() { return description; }
void Set::setId(uint id) { this->id = id; }
void Set::setName(std::string name) { this->name = name; }
void Set::setAuthor(std::string author) { this->author = author; }
void Set::setDescription(std::string desc) { this->description = desc; }

void Set::add(Card card) {
    cards.push_back(card);
}

void Set::add(Card card, uint i) {
    if (i < 0 || i > getSize()) {
        throw std::out_of_range("Index out of bounds in add()");
    }
    cards.insert(cards.begin() + i, card);
}

void Set::set(uint i, Card card) {
    if (i < 0 || i >= getSize()) {
        throw std::out_of_range("Index out of bounds in set()");
    }
}

Card Set::get(uint i) {
    if (i < 0 || i >= getSize()) {
        throw std::out_of_range("Index out of bounds in get()");
    }
    return cards[i];
}

int Set::getSize() {
    return static_cast<int>(cards.size());
}

nlohmann::json Set::toJSON()
{
    nlohmann::json json = {
        {"name", getName()},
        {"id", getId()},
        {"auth", getAuthor()},
        {"desc", getDescription()},
        {"cards", nlohmann::json::array()}
    };
    for(int i = 0; i < getSize(); i++)
    {
        nlohmann::json card = {
            {"t", get(i).getTerm()},
            {"d", get(i).getDefinition()}
        };
        json["cards"].push_back(card);
    }
    return json;
}
