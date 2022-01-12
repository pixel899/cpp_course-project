#include "binary_tree.cpp"
#include "railway_station.h"

template class binary_tree<int, std::shared_ptr<train_data>>;
template class binary_tree<std::string, std::vector<std::shared_ptr<train_data>>>;
