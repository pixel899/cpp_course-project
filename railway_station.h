#pragma once

#include "train_data.h"
#include "binary_tree.h"

#include <memory>
#include <vector>

class railway_station {
private:
    binary_tree<int, std::shared_ptr<train_data>> trains;
    binary_tree<std::string, std::vector<std::shared_ptr<train_data>>> stations;
public:
    railway_station();

    void read_from_file(std::string filename);

    void add_train_data(const train_data& data);

    bool is_train_exists(int id);
    std::shared_ptr<train_data> get_train_info(int id);

    bool is_station_exists(const std::string& name);
    const std::vector<std::shared_ptr<train_data>>& get_station_info(const std::string& name);

    const binary_tree<std::string, std::vector<std::shared_ptr<train_data>>>* get_stations();
};

