#ifndef DATABASECLIENT_HPP
#define DATABASECLIENT_HPP

#include <cstdint>
#include <string>
#include <vector>
#include "set.hpp"
#include <filesystem>

class DatabaseClient {
    private:
        std::filesystem::path path;
    public:
        DatabaseClient();
        std::vector<Set*> getSets();
        Set* getSet(uint id);
        bool deleteSet(uint);
        bool saveSet(Set*);
        bool purge();
};

#endif