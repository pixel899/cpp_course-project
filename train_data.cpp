#include "train_data.h"

#include <array>
#include <sstream>
#include <iomanip>
using namespace std;

train_data::train_data()
    : id(0), destination_station("") {
}

train_data::train_data(const train_data& data)
    : id                 (data.id),
      destination_station(data.destination_station),
      destination_time   (data.destination_time) {
}

istream& operator >> (istream& in, train_data& data) {
    in >> data.id >> data.destination_station;
    std::array<char, 30> a;
    in.getline(&a[0], 30);

    istringstream ss(&a[0]);
    ss >> std::get_time(&data.destination_time, "%Y-%b-%d %H:%M:%S");
    return in;
}

