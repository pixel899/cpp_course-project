#pragma once

#include <string>
#include <ctime>
#include <istream>

class train_data {
public:
    int id;
    std::string destination_station;
    std::tm destination_time;

    train_data();
    train_data(const train_data& data);

    friend std::istream& operator >> (std::istream& in, train_data& data);
};


