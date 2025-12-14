#include "databaseClient.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include "set.hpp"
#include "card.hpp"

DatabaseClient::DatabaseClient() {
    const char* env_path = std::getenv("FCARDS_PATH");
    if(env_path != nullptr)
    {
        if(!std::filesystem::exists(env_path))
        {
            std::cerr << "No such directory: " << env_path << std::endl;
            exit(1);
        }
        else path = std::filesystem::path(env_path);
    }
    else {
        std::filesystem::path dir =
        std::filesystem::path(std::getenv("HOME"))
        / ".local" / "share" / "fcards";
        if(!std::filesystem::exists(dir)) {
            std::filesystem::create_directories(dir);
        }
        if(std::filesystem::exists(dir)) path = dir;
        else std::cerr << "Failed to create default directory: " << dir << std::endl;
    };
    std::cout << "Database path set to: " << path << std::endl;
}

std::vector<Set*> DatabaseClient::getSets() { 
    std::vector<Set*> sets;
    for(const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path))
    {
        if(!entry.path().has_filename()) continue;
        std::string filename = entry.path().filename().string();
        std::string extension(".json");
        if(filename.compare(filename.size() - extension.size(), extension.size(), extension)) continue;
        int id = atoi(filename.erase(filename.size()-extension.size()).c_str());
        Set* set = getSet(static_cast<uint>(id));
        if(set == nullptr) continue;
        sets.push_back(set);
    }
    return sets;
};

//delete pointer after use!
Set* DatabaseClient::getSet(uint id) {
    std::ifstream file(std::filesystem::path(path / (std::to_string(id)+".json")));
    if(!file.is_open()) return nullptr;
    nlohmann::json json;
    try { file >> json; } catch(nlohmann::json::parse_error& e) { return nullptr; };
    file.close();
    Set* set = new Set(json["name"]);
    set->setId(json["id"]);
    set->setAuthor(json["auth"]);
    set->setDescription(json["desc"]);
    for(int i = 0; i < json["cards"].size(); i++)
    {
        set->add(Card(json["cards"][i]["t"], json["cards"][i]["d"]));
    }
    return set;
};

bool DatabaseClient::deleteSet(uint id) {
    std::filesystem::path f_path = path / (std::to_string(id)+".json");
    return std::filesystem::remove(f_path);
};

bool DatabaseClient::saveSet(Set* set) {
    std::filesystem::path f_path = path / (std::to_string(set->getId())+".json");
    std::cout << "Saving set \"" << set->getName() << "\" to " << f_path << std::endl;
    std::ofstream file(f_path);
    if(file.is_open()) { file << set->toJSON(); return 0; }
    else { return 1; }
};

bool DatabaseClient::purge() {
    try {
        for(const std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path)) {
            std::filesystem::remove_all(entry);
        };
        return true;
    } catch(const std::filesystem::filesystem_error& e) {
        return false;
    }
}