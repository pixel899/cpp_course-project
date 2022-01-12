#include <iostream>
#include <fstream>
#include <iomanip>
#include "railway_station.h"
using namespace std;

int main() 
	{
		railway_station s;
		s.read_from_file("tests/input_data_1.txt");

		ifstream in("tests/queries_1.txt");
		ofstream out("output.txt");

		int n;
		in >> n;

		for (int i = 0; i < n; i++) {
			string query_type;
			in >> query_type;
			if (query_type == "train_info") {
				int id;
				in >> id;
				if (s.is_train_exists(id)) {
					auto p = s.get_train_info(id);
					out << p->destination_station << " " << put_time(&p->destination_time, "%c") << "\n";
				}
				else {
					out << "No train with id \"" << id << "\"\n";
				}
			}
			else
				if (query_type == "station_info") {
					string name;
					in >> name;
					if (s.is_station_exists(name)) {
						auto &res = s.get_station_info(name);
						out << "All trains arriving to station:\n";
						for (auto p : res) {
							out << "  " << p->id << " " << put_time(&p->destination_time, "%c") << "\n";
						}
					}
					else {
						out << "No station with name \"" << name << "\"\n";
					}
				}
		}
		return 0;
	}

      
   

