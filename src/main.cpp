#include "crow.h"
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <sys/types.h>
#include <vector>
#include "set.hpp"
#include "card.hpp"
#include "json.hpp"
#include "databaseClient.hpp"

Set* createSet(crow::json::rvalue* body) {
std::string name, author, description;
    std::cout << *body << "\n";

    if(body->has("name") && (*body)["name"].t() == crow::json::type::String && (*body)["name"].s().size() > 0) name = (*body)["name"].s();
    else name = "New Set";
    if(body->has("auth") && (*body)["auth"].t() == crow::json::type::String && (*body)["auth"].s().size() > 0) author = (*body)["auth"].s();
    else author = "Anonymous";
    if(body->has("desc") && (*body)["desc"].t() == crow::json::type::String && (*body)["desc"].s().size() > 0) description = (*body)["desc"].s();
    else description = "";

    Set *set = new Set(name);
    set->setAuthor(author);
    set->setDescription(description);
    std::cout << " IS " << set->getName() << "\n";
    std::cout << "AUTHOR IS " << set->getAuthor() << "\n";
    if(body->has("cards") && (*body)["cards"].t() == crow::json::type::List)
    {
        for(int i = 0; i < (*body)["cards"].size(); i++) {
            std::string t, d;
            if((*body)["cards"][i].has("t") && (*body)["cards"][i]["t"].t() == crow::json::type::String)
            { t = (*body)["cards"][i]["t"].s(); }
            if((*body)["cards"][i].has("d") && (*body)["cards"][i]["d"].t() == crow::json::type::String)
            { d = (*body)["cards"][i]["d"].s(); }
            Card card(t,d);
            set->add(card);
        }
    }
    return set;
}

int main()
{
    crow::SimpleApp app;
    DatabaseClient db;

    CROW_ROUTE(app, "/").methods(crow::HTTPMethod::Get)
    ([](){
        crow::mustache::context ctx;
        ctx["sets"] = crow::json::wvalue::list(10);
        auto page = crow::mustache::load("home.html");
        return page.render(ctx);
    });

    CROW_ROUTE(app, "/sets").methods(crow::HTTPMethod::Get)
    ([](const crow::request& req, crow::response& res) {
        res.redirect("/");
        res.end();
    });

    CROW_ROUTE(app, "/create").methods(crow::HTTPMethod::Get)
    ([](){
        crow::mustache::context ctx;
        auto page = crow::mustache::load("create.html");
        return page.render(ctx);
    });

    CROW_ROUTE(app, "/sets/<int>").methods(crow::HTTPMethod::Get)
    ([&db](int id){
        crow::mustache::context ctx;
        auto page = crow::mustache::load("set.html");
        return page.render(ctx);
    });

    CROW_ROUTE(app, "/sets/<int>/raw").methods(crow::HTTPMethod::Get)
    ([&db](const crow::request& req, crow::response& res, int id){
        Set* set = db.getSet(static_cast<uint>(id));
        if(set == nullptr) res.code = 400;
        else 
        {
            res.set_header("Content-Type", "application/json");
            res.write(set->toJSON().dump());
        }
        delete set; //clean up
        res.end();
    });

    CROW_ROUTE(app, "/sets/raw").methods(crow::HTTPMethod::Get)
    ([&db](const crow::request& req, crow::response& res) {
        res.set_header("Content-Type", "application/json");
        nlohmann::json res_json = nlohmann::json::array();
        std::vector<Set*> sets = db.getSets();
        for(int i = 0; i < sets.size(); i++)
        { res_json.push_back(sets[i]->toJSON()); }
        res.write(res_json.dump());
        for(auto set : sets) delete set; //clean up
        res.end();
    });
    
    CROW_ROUTE(app, "/sets").methods(crow::HTTPMethod::Post)
    ([&db](const crow::request& req){
        //Create set
        crow::json::rvalue body = crow::json::load(req.body);
        if (!body) return crow::response(400, "JSON required");
        Set* set = createSet(&body);
        db.saveSet(set);
        delete set;
        return crow::response(200);
    });

    CROW_ROUTE(app, "/sets/<int>").methods(crow::HTTPMethod::Put)
    ([&db](const crow::request& req, int id){
        crow::json::rvalue body = crow::json::load(req.body);
        if (!body) return crow::response(400, "JSON required");
        db.deleteSet(id);
        Set* set = createSet(&body);
        set->setId(id);
        bool r = db.saveSet(set);
        delete set;
        if(r) return crow::response(200);
        else return crow::response(400);
    });

    CROW_ROUTE(app, "/sets/<int>/delete").methods(crow::HTTPMethod::Get)
    ([&db](const crow::request& req, crow::response& res, int id){
        if(db.deleteSet(id)) res.redirect("/"); else { res.code = 500; }
        res.end();
    });

    CROW_ROUTE(app, "/purge").methods(crow::HTTPMethod::Get)
    ([&db](const crow::request& req, crow::response& res){
        if(db.purge()) res.redirect("/"); else { res.code = 500; }
        res.end();
    });
   
    app.port(8080).run();
    
}