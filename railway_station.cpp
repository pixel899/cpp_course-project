#include "railway_station.h"
#include <fstream>

using namespace std;

const binary_tree<string, vector<shared_ptr<train_data>>>* railway_station::get_stations() {
    return &stations;
}

railway_station::railway_station() {
}

void railway_station::read_from_file(string filename) {
    ifstream in(filename);
    int n;
    in >> n;
    for (int i = 0; i < n; i++) {
        train_data data;
        in >> data;
        add_train_data(data);
    }
}

void railway_station::add_train_data(const train_data& data) {
    shared_ptr<train_data> p = make_shared<train_data>(data);
    trains.insert(p->id, p);

    auto it = stations.find(p->destination_station);
    if (it == stations.end()) {
        it = stations.insert(p->destination_station, {});
    }
    (*it).second.push_back(p);
}

bool railway_station::is_train_exists(int id) {
    return (trains.find(id) != trains.end());
}

shared_ptr<train_data> railway_station::get_train_info(int id) {
    return (*trains.find(id)).second;
}

bool railway_station::is_station_exists(const string& name) {
    return (stations.find(name) != stations.end());
}

const vector<shared_ptr<train_data>>& railway_station::get_station_info(const string& name) {
    return (*stations.find(name)).second;
}

