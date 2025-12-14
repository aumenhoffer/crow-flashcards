#ifndef SET_HPP
#define SET_HPP

#include <vector>
#include <string>
#include "card.hpp"
#include "json.hpp"

class Set {
    private:
        std::string name;
        uint id;
        std::string author;
        std::string description;
        std::vector<Card> cards;
    public:
        Set(const std::string name);
        std::string getName();
        uint getId();
        std::string getAuthor();
        std::string getDescription();
        void setId(uint);
        void setName(std::string name);
        void setAuthor(std::string name);
        void setDescription(std::string name);
        int getSize();
        void add(Card card);
        void add(Card card, uint i);
        void set(uint i, Card card);
        Card get(uint i);
        nlohmann::json toJSON();
};

#endif